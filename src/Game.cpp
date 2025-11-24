#include "Game.hpp"
#include "ui/PantallaMenuPrincipal.hpp"
#include "DatosPruebaRecetario.hpp" // NECESARIO para los datos de prueba
#include <iostream>
#include <stdexcept>
#include <string>

Game::Game() : ventana(nullptr), renderer(nullptr), estaCorriendo(false) {
    try {
        inicializarSDL();
        
        // =========================================================
        // ⚡ CARGA AUTOMÁTICA DE 1000 RECETAS (BYPASS)
        // =========================================================
        std::cout << "[INIT] Generando 1000 recetas de prueba en memoria..." << std::endl;
        
        // 1. Obtenemos las recetas base (aprox 12)
        ListaDoblementeLigada<Receta> listaBase = DatosPruebaRecetario::obtenerRecetasDeEjemplo();
        
        int cantidadObjetivo = 1000;
        int cantidadActual = 0;
        
        // 2. Bucle para multiplicar las recetas hasta llegar a 1000
        while (cantidadActual < cantidadObjetivo) {
            for (int i = 0; i < listaBase.obtenerCantidadElementos(); ++i) {
                if (cantidadActual >= cantidadObjetivo) break;
                
                // Copiamos una receta base
                Receta nuevaReceta = listaBase.obtenerEnPosicion(i);
                
                // Le cambiamos el nombre para que sea única (ej: "Mole #15")
                // Esto es importante para que las búsquedas y ordenamientos se noten
                std::string nombreUnico = nuevaReceta.obtenerNombrePlatillo() + " #" + std::to_string(cantidadActual + 1);
                nuevaReceta.establecerNombrePlatillo(nombreUnico);
                
                // La guardamos en el manejador principal
                try {
                    manejadorRecetas.agregarRecetaNueva(nuevaReceta);
                    cantidadActual++;
                } catch (...) {
                    // Ignorar duplicados si ocurren
                }
            }
        }
        
        std::cout << "[INIT] Carga completada. Total recetas: " << manejadorRecetas.obtenerCantidadRecetas() << std::endl;
        // =========================================================
        
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
    
    std::cout << "[SISTEMA] Subsistemas SDL iniciados." << std::endl;
}

void Game::cargarDatos() {
    // Deshabilitado temporalmente para usar la generación automática
    /*
    try {
        manejadorRecetas.cargarRecetasDesdeArchivo(RUTA_ARCHIVO_RECETAS);
    } catch (...) {
        std::cout << "[DATOS] Error o archivo no encontrado." << std::endl;
    }
    */
}

void Game::limpiar() {
    // Evitamos guardar las 1000 recetas de prueba en el archivo real al salir
    // para no ensuciar tu base de datos persistente.
    // try { manejadorRecetas.guardarRecetasAarchivo(RUTA_ARCHIVO_RECETAS); } catch (...) {}

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
