#include "Game.hpp"
#include "ui/PantallaMenuPrincipal.hpp"
#include <iostream>
#include <stdexcept>
// #include <SDL3_image/SDL_image.h> // No es necesario explícitamente aquí si no usamos IMG_Init

Game::Game() : ventana(nullptr), renderer(nullptr), estaCorriendo(false) {
    try {
        inicializarSDL();
        
        // Inicio vacío (sin cargar datos automáticos)
        // cargarDatos(); 
        
        this->estaCorriendo = true;
        
        std::cout << "[DEBUG] Creando estado Menu Principal..." << std::endl;
        pushEstado(new PantallaMenuPrincipal()); 
        std::cout << "[SISTEMA] Inicializacion completa." << std::endl;
        
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
    
    // NOTA: En SDL3_image, IMG_Init ya no es necesario/no existe.
    // La librería se auto-inicializa al usar IMG_Load.
    
    std::cout << "[SISTEMA] Subsistemas SDL iniciados." << std::endl;
}

void Game::cargarDatos() {
    try {
        manejadorRecetas.cargarRecetasDesdeArchivo(RUTA_ARCHIVO_RECETAS);
    } catch (...) {
        std::cout << "[DATOS] Error o archivo no encontrado." << std::endl;
    }
}

void Game::limpiar() {
    try {
        // Guardado manual recomendado, pero dejamos el automático por seguridad
        manejadorRecetas.guardarRecetasAarchivo(RUTA_ARCHIVO_RECETAS);
    } catch (const std::exception& e) {
        std::cerr << "Error al guardar recetas: " << e.what() << std::endl;
    }

    while (!estadosJuego.estaVacia()) {
        try {
            GameState* estadoActual = estadosJuego.consultarTope();
            if (estadoActual) {
                estadoActual->cleanup();
                delete estadoActual;
            }
            estadosJuego.desapilar();
        } catch (...) {
             break; 
        }
    }

    if (renderer) { SDL_DestroyRenderer(renderer); renderer = nullptr; }
    if (ventana) { SDL_DestroyWindow(ventana); ventana = nullptr; }
    
    TTF_Quit();
    // IMG_Quit(); // Eliminado por ser obsoleto
    SDL_Quit();
    
    std::cout << "[SISTEMA] Recursos liberados. Adios." << std::endl;
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
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);

    if (!estadosJuego.estaVacia()) {
        estadosJuego.consultarTope()->render(*this);
    }

    SDL_RenderPresent(renderer);
}

void Game::pushEstado(GameState* estado) {
    if (!estado) return;
    estadosJuego.apilar(estado);
    estadosJuego.consultarTope()->init(*this);
}

void Game::popEstado() {
    if (!estadosJuego.estaVacia()) {
        GameState* estadoAeliminar = estadosJuego.consultarTope();
        if (estadoAeliminar) estadoAeliminar->cleanup();
        estadosJuego.desapilar();
        delete estadoAeliminar;
    }
}

void Game::cambiarEstado(GameState* estadoNuevo) {
    if (!estadoNuevo) return;
    
    GameState* estadoAnterior = nullptr;
    if (!estadosJuego.estaVacia()) {
        estadoAnterior = estadosJuego.consultarTope();
        estadosJuego.desapilar();
    }
    
    if (estadoAnterior) {
        estadoAnterior->cleanup();
        delete estadoAnterior;
    }
    
    estadosJuego.apilar(estadoNuevo);
    estadosJuego.consultarTope()->init(*this);
}

void Game::run() {
    std::cout << "[RUN] Iniciando bucle principal..." << std::endl;
    while (estaCorriendo) {
        if (estadosJuego.estaVacia()) {
            estaCorriendo = false;
            continue;
        }

        procesarEventos();
        actualizar();
        dibujar();
        
        SDL_Delay(16);
    }
    std::cout << "[RUN] Bucle finalizado." << std::endl;
}
