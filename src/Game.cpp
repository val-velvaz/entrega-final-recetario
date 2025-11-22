#include "Game.hpp"
#include "ui/PantallaMenuPrincipal.hpp"
#include <iostream>
#include <stdexcept>

Game::Game() : ventana(nullptr), renderer(nullptr), estaCorriendo(false) {
    try {
        inicializarSDL();
        cargarDatos();
        
        this->estaCorriendo = true;
        
        pushEstado(new PantallaMenuPrincipal()); 
        
        std::cout << "Sistema inicializado correctamente." << std::endl;
        
    } catch (const std::exception& e) {
        std::cerr << "Error fatal durante la inicializacion: " << e.what() << std::endl;
        this->estaCorriendo = false;
    }
}

Game::~Game() {
    limpiar();
}

void Game::inicializarSDL() {
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        throw std::runtime_error(std::string("Error al inicializar SDL: ") + SDL_GetError());
    }

    ventana = SDL_CreateWindow("Recetario Interactivo (EDA)", 1024, 768, 0);
    if (!ventana) throw std::runtime_error("Error creando ventana: " + std::string(SDL_GetError()));

    renderer = SDL_CreateRenderer(ventana, NULL);
    if (!renderer) throw std::runtime_error("Error creando renderer: " + std::string(SDL_GetError()));
    
    SDL_SetRenderVSync(renderer, 1);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    if (!TTF_Init()) {
        throw std::runtime_error("Error al inicializar TTF: " + std::string(SDL_GetError()));
    }
}

void Game::cargarDatos() {
    try {
        manejadorRecetas.cargarRecetasDesdeArchivo(RUTA_ARCHIVO_RECETAS);
        std::cout << "Recetas cargadas: " << manejadorRecetas.obtenerCantidadRecetas() << std::endl;
    } catch (...) {
        std::cout << "Iniciando recetario vacio (Archivo no encontrado o nuevo)." << std::endl;
    }
}

void Game::limpiar() {
    try {
        std::cout << "Guardando recetas..." << std::endl;
        manejadorRecetas.guardarRecetasAarchivo(RUTA_ARCHIVO_RECETAS);
        std::cout << "Recetas guardadas exitosamente." << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error al guardar recetas: " << e.what() << std::endl;
    }

    // Limpiar estados de la FSM
    while (!estadosJuego.estaVacia()) {
        try {
            GameState* estadoActual = estadosJuego.consultarTope();
            if (estadoActual) {
                estadoActual->cleanup();
                delete estadoActual;
            }
        } catch (const std::exception& e) {
            std::cerr << "Error critico durante cleanup del estado: " 
                      << e.what() << std::endl;
        } catch (...) {
            std::cerr << "Error critico desconocido durante cleanup del estado." << std::endl;
        }
        
        try {
            estadosJuego.desapilar();
        } catch (const std::exception& e) {
             std::cerr << "Error critico al desapilar estado: " 
                       << e.what() << std::endl;
             break; 
        }
    }

    // Destruir recursos de SDL
    if (renderer) {
        SDL_DestroyRenderer(renderer);
        renderer = nullptr;
    }
    if (ventana) {
        SDL_DestroyWindow(ventana);
        ventana = nullptr;
    }
    
    TTF_Quit();
    SDL_Quit();
    
    std::cout << "SDL limpiado correctamente. Adios." << std::endl;
}

void Game::procesarEventos() {
    if (!estadosJuego.estaVacia()) {
        estadosJuego.consultarTope()->handleEvents(*this);
    }
}

void Game::actualizar() {
    if (!estadosJuego.estaVacia()) {
        estadosJuego.consultarTope()->update(*this);
    }
}

void Game::dibujar() {
    SDL_SetRenderDrawColor(renderer, 255, 230, 230, 255);
    SDL_RenderClear(renderer);

    if (!estadosJuego.estaVacia()) {
        estadosJuego.consultarTope()->render(*this);
    }

    SDL_RenderPresent(renderer);
}

void Game::pushEstado(GameState* estado) {
    if (!estado) {
        std::cerr << "[ERROR] Intento de apilar estado nulo!" << std::endl;
        return;
    }
    estadosJuego.apilar(estado);
    estadosJuego.consultarTope()->init(*this);
}

// ❌ MÉTODO CON BUG - CORREGIDO ✅
void Game::popEstado() {
    if (!estadosJuego.estaVacia()) {
        // 1. Guardamos el puntero ANTES de desapilar
        GameState* estadoAeliminar = estadosJuego.consultarTope();
        
        // 2. Limpiamos recursos del estado
        if (estadoAeliminar) {
            estadoAeliminar->cleanup();
        }
        
        // 3. Desapilamos (esto solo quita el puntero de la pila)
        estadosJuego.desapilar();
        
        // 4. AHORA sí liberamos la memoria
        delete estadoAeliminar;
        estadoAeliminar = nullptr; // Buena práctica
        
        std::cout << "[DEBUG] Estado eliminado correctamente" << std::endl;
    }
}

// ❌ MÉTODO CON BUG CRÍTICO - CORREGIDO ✅
void Game::cambiarEstado(GameState* estadoNuevo) {
    if (!estadoNuevo) {
        std::cerr << "[ERROR] Intento de cambiar a estado nulo!" << std::endl;
        return;
    }
    
    std::cout << "[DEBUG] Cambiando estado..." << std::endl;
    
    // 1. Guardamos el puntero del estado actual ANTES de hacer nada
    GameState* estadoAnterior = nullptr;
    if (!estadosJuego.estaVacia()) {
        estadoAnterior = estadosJuego.consultarTope();
    }
    
    // 2. Limpiamos el estado anterior
    if (estadoAnterior) {
        estadoAnterior->cleanup();
    }
    
    // 3. Desapilamos (solo removemos el puntero de la pila)
    if (!estadosJuego.estaVacia()) {
        estadosJuego.desapilar();
    }
    
    // 4. Liberamos memoria del estado anterior
    if (estadoAnterior) {
        delete estadoAnterior;
        estadoAnterior = nullptr;
    }
    
    // 5. Apilamos e inicializamos el nuevo estado
    estadosJuego.apilar(estadoNuevo);
    estadosJuego.consultarTope()->init(*this);
    
    std::cout << "[DEBUG] Estado cambiado exitosamente" << std::endl;
}

void Game::run() {
    while (estaCorriendo) {
        if (estadosJuego.estaVacia()) {
            std::cout << "Diagnostico: Pila de estados vacia detectada. Terminando aplicacion..." << std::endl;
            estaCorriendo = false;
            continue;
        }

        procesarEventos();
        actualizar();
        dibujar();
    }
}