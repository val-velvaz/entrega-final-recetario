#include "ui/PantallaGestionArchivos.hpp"
#include "ui/PantallaVisualizarRecetas.hpp"
#include "Game.hpp"
#include "persistence/RutasAssets.hpp"
#include "utils/RenderizadorTextos.hpp"
#include "DatosPruebaRecetario.hpp"
#include <iostream>
#include <filesystem>
#include <future>
#include <chrono>

namespace fs = std::filesystem;

PantallaGestionArchivos::PantallaGestionArchivos(bool esModoGuardar)
    : fuenteTitulo(nullptr), fuenteLista(nullptr), fuenteSettings(nullptr), fuenteCargando(nullptr), 
      texTitulo(nullptr), rectTitulo({0,0,0,0}), modoGuardar(esModoGuardar), 
      btnNuevoArchivo(nullptr), inputNuevoArchivo(nullptr), btnCargarTest(nullptr), btnVolver(nullptr),
      btnSeparador(nullptr), texSeparador(nullptr), separadorActual(""),
      cargandoDatos(false), texCargando(nullptr), rectCargando({0,0,0,0}), tiempoInicio(0) {}

PantallaGestionArchivos::~PantallaGestionArchivos() {
    cleanup();
}

void PantallaGestionArchivos::init(Game& game) {
    SDL_Renderer* renderer = game.getRenderer();
    SDL_Window* window = game.getWindow();
    int w, h;
    SDL_GetRenderOutputSize(renderer, &w, &h);

    tiempoInicio = SDL_GetTicks();

    fuenteTitulo = TTF_OpenFont(RutasAssets::obtenerRutaFuenteNegrita().c_str(), 36);
    fuenteLista = TTF_OpenFont(RutasAssets::obtenerRutaFuenteRegular().c_str(), 20);
    fuenteCargando = TTF_OpenFont(RutasAssets::obtenerRutaFuenteNegrita().c_str(), 28);

    std::string titulo = modoGuardar ? "GUARDAR ARCHIVO" : "CARGAR ARCHIVO";
    texTitulo = RenderizadorTextos::renderizarTexto(renderer, fuenteTitulo, titulo, {50, 50, 70, 255});
    if (texTitulo) {
        rectTitulo = { (w - (float)texTitulo->w)/2.0f, 30.0f, (float)texTitulo->w, (float)texTitulo->h };
    }

    escanearDirectorio(renderer);

    float yBottom = h - 100.0f;
    
    if (modoGuardar) {
        inputNuevoArchivo = new CajaDeTexto(window, w/2 - 200, yBottom, 300, 40, fuenteLista);
        inputNuevoArchivo->establecerPlaceholder("Nombre nuevo archivo...");
        btnNuevoArchivo = new Boton(renderer, "assets/images/icons/archivo_nuevo.png", w/2 + 120, yBottom, 40, 40);
    } else {
        btnCargarTest = new Boton(renderer, "assets/images/icons/archivo_test.png", w/2 - 25, yBottom, 50, 50); 
    }

    btnVolver = new Boton(renderer, RutasAssets::obtenerRutaIconoBotonVolver(), 50, yBottom, 50, 50);
    
    cargandoDatos = false;
}

void PantallaGestionArchivos::escanearDirectorio(SDL_Renderer* renderer) {
    for(auto& b : listaBotonesArchivos) {
        if (b.btn) delete b.btn;
        if (b.label) SDL_DestroyTexture(b.label);
    }
    listaBotonesArchivos.clear();

    std::string pathSaves = "assets/saves";
    if (!fs::exists(pathSaves)) fs::create_directories(pathSaves);

    float yPos = 100.0f;
    int w, h;
    SDL_GetRenderOutputSize(renderer, &w, &h);

    int fileCount = 0;
    for (const auto& entry : fs::directory_iterator(pathSaves)) {
        if (entry.path().extension() == ".dat") {
            if (fileCount >= 3) continue;

            std::string filename = entry.path().filename().string();
            
            BotonArchivo item;
            item.btn = new Boton(renderer, "assets/images/icons/boton_guardar.png", w/2 - 200, yPos, 40, 40);
            item.nombreArchivo = filename;
            
            item.label = RenderizadorTextos::renderizarTexto(renderer, fuenteLista, filename, {0,0,0,255});
            if (item.label) {
                item.rectLabel = { (float)w/2 - 140, yPos + 10, (float)item.label->w, (float)item.label->h };
            }

            listaBotonesArchivos.push_back(item);
            yPos += 60.0f;
            fileCount++;
        }
    }
}

void PantallaGestionArchivos::cleanup() {
    if (tareaIO.valid()) {
        tareaIO.wait();
    }
    
    if (texTitulo) { SDL_DestroyTexture(texTitulo); texTitulo = nullptr; }
    if (texCargando) { SDL_DestroyTexture(texCargando); texCargando = nullptr; }
    if (fuenteTitulo) { TTF_CloseFont(fuenteTitulo); fuenteTitulo = nullptr; }
    if (fuenteLista) { TTF_CloseFont(fuenteLista); fuenteLista = nullptr; }
    if (fuenteCargando) { TTF_CloseFont(fuenteCargando); fuenteCargando = nullptr; }

    for(auto& b : listaBotonesArchivos) {
        if (b.btn) { delete b.btn; b.btn = nullptr; }
        if (b.label) { SDL_DestroyTexture(b.label); b.label = nullptr; }
    }
    listaBotonesArchivos.clear();

    if (btnNuevoArchivo) { delete btnNuevoArchivo; btnNuevoArchivo = nullptr; }
    if (inputNuevoArchivo) { delete inputNuevoArchivo; inputNuevoArchivo = nullptr; }
    if (btnCargarTest) { delete btnCargarTest; btnCargarTest = nullptr; }
    if (btnVolver) { delete btnVolver; btnVolver = nullptr; }
}

void PantallaGestionArchivos::handleEvents(Game& game) {
    SDL_Event evento;
    
    // CORRECCION: Siempre procesar eventos para mantener la ventana viva
    while (SDL_PollEvent(&evento)) {
        if (evento.type == SDL_EVENT_QUIT) {
            game.setEstaCorriendo(false);
            return;
        }

        // Si estamos cargando, ignoramos la INTERACCIÓN, pero ya procesamos el evento de sistema
        if (cargandoDatos) continue;

        if (inputNuevoArchivo) inputNuevoArchivo->manejarEvento(evento);

        if (evento.type == SDL_EVENT_MOUSE_BUTTON_DOWN && evento.button.button == SDL_BUTTON_LEFT) {
            
            // Debounce
            if (SDL_GetTicks() - tiempoInicio < 300) { 
                return;
            }

            float mx = evento.button.x;
            float my = evento.button.y;

            if (btnVolver && btnVolver->estaPresionado(mx, my)) {
                game.popEstado();
                return;
            }

            for (const auto& item : listaBotonesArchivos) {
                if (item.btn->estaPresionado(mx, my)) {
                    seleccionarArchivo(game, "assets/saves/" + item.nombreArchivo);
                    return; 
                }
            }

            if (modoGuardar && btnNuevoArchivo && btnNuevoArchivo->estaPresionado(mx, my)) {
                crearYSeleccionar(game);
            }

            if (!modoGuardar && btnCargarTest && btnCargarTest->estaPresionado(mx, my)) {
                cargarDatosPrueba(game);
            }
        }
    }
}

void PantallaGestionArchivos::seleccionarArchivo(Game& game, const std::string& ruta) {
    if (cargandoDatos) return;
    
    cargandoDatos = true;
    rutaPendiente = ruta;
    modoGuardarPendiente = modoGuardar;
    
    if (texCargando) SDL_DestroyTexture(texCargando);
    texCargando = RenderizadorTextos::renderizarTexto(
        game.getRenderer(), 
        fuenteCargando, 
        modoGuardar ? "GUARDANDO..." : "CARGANDO RECETAS...", 
        {255, 100, 100, 255}
    );
    
    if (texCargando) {
        int w, h;
        SDL_GetRenderOutputSize(game.getRenderer(), &w, &h);
        rectCargando = { 
            (w - (float)texCargando->w) / 2.0f, 
            (h - (float)texCargando->h) / 2.0f, 
            (float)texCargando->w, 
            (float)texCargando->h 
        };
    }
    
    tareaIO = std::async(std::launch::async, [this, &game, ruta]() {
        try {
            if (modoGuardarPendiente) {
                game.getManejadorRecetas().guardarRecetasAarchivo(ruta);
            } else {
                game.getManejadorRecetas().cargarRecetasDesdeArchivo(ruta);
            }
        } catch (const std::exception& e) {
            std::cerr << "[ERROR I/O ASYNC] " << e.what() << std::endl;
        }
    });
}

void PantallaGestionArchivos::crearYSeleccionar(Game& game) {
    std::string nombre = inputNuevoArchivo->obtenerTexto();
    if (nombre.empty()) return;
    
    if (nombre.find(".dat") == std::string::npos) nombre += ".dat";
    seleccionarArchivo(game, "assets/saves/" + nombre);
}

void PantallaGestionArchivos::cargarDatosPrueba(Game& game) {
    ListaDoblementeLigada<Receta> datosTest = DatosPruebaRecetario::obtenerRecetasDeEjemplo();
    for (int i = 0; i < datosTest.obtenerCantidadElementos(); ++i) {
        try {
            game.getManejadorRecetas().agregarRecetaNueva(datosTest.obtenerEnPosicion(i));
        } catch (...) {} 
    }
    game.popEstado();
}

void PantallaGestionArchivos::update(Game& game) { 
    if (cargandoDatos && tareaIO.valid()) {
        if (tareaIO.wait_for(std::chrono::milliseconds(0)) == std::future_status::ready) {
            tareaIO.get();
            cargandoDatos = false;
            game.popEstado();
            return;
        }
    }
    
    if (!cargandoDatos) {
        for(auto& b : listaBotonesArchivos) b.btn->actualizar();
        if(btnNuevoArchivo) btnNuevoArchivo->actualizar();
        if(btnCargarTest) btnCargarTest->actualizar();
        if(btnVolver) btnVolver->actualizar();
    }
}

void PantallaGestionArchivos::render(Game& game) {
    SDL_Renderer* r = game.getRenderer();
    int w, h;
    SDL_GetRenderOutputSize(r, &w, &h);
    
    SDL_SetRenderDrawColor(r, 230, 235, 240, 255);
    SDL_RenderClear(r);

    if (cargandoDatos) {
        SDL_SetRenderDrawBlendMode(r, SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(r, 0, 0, 0, 180);
        SDL_FRect overlay = {0, 0, (float)w, (float)h};
        SDL_RenderFillRect(r, &overlay);
        
        if (texCargando) {
            SDL_RenderTexture(r, texCargando, nullptr, &rectCargando);
        }
        
        float tiempo = SDL_GetTicks() / 100.0f;
        int puntos = ((int)tiempo % 4);
        std::string puntosStr(puntos, '.');
        
        SDL_Texture* tPuntos = RenderizadorTextos::renderizarTexto(
            r, fuenteCargando, puntosStr, {255, 255, 255, 255}
        );
        
        if (tPuntos) {
            SDL_FRect rP = { rectCargando.x + rectCargando.w + 10, rectCargando.y, (float)tPuntos->w, (float)tPuntos->h };
            SDL_RenderTexture(r, tPuntos, nullptr, &rP);
            SDL_DestroyTexture(tPuntos);
        }
        
        SDL_SetRenderDrawBlendMode(r, SDL_BLENDMODE_NONE);
        
        // CORRECCION: Eliminado SDL_RenderPresent para evitar parpadeo
        return;
    }

    if (texTitulo) SDL_RenderTexture(r, texTitulo, nullptr, &rectTitulo);

    for (const auto& item : listaBotonesArchivos) {
        SDL_SetRenderDrawColor(r, 255, 255, 255, 200);
        SDL_FRect row = { 100.0f, item.rectLabel.y - 5, 824.0f, 40.0f };
        SDL_RenderFillRect(r, &row);
        item.btn->render(r);
        SDL_RenderTexture(r, item.label, nullptr, &item.rectLabel);
    }

    if (modoGuardar) {
        if (inputNuevoArchivo) inputNuevoArchivo->render(r);
        if (btnNuevoArchivo) btnNuevoArchivo->render(r);
        
        SDL_Surface* sLbl = TTF_RenderText_Blended(fuenteLista, "Crear nuevo archivo:", 0, {100, 100, 100, 255});
        if (sLbl) {
            SDL_Texture* tLbl = SDL_CreateTextureFromSurface(r, sLbl);
            SDL_FRect rLbl = { 200.0f, h - 145.0f, (float)sLbl->w, (float)sLbl->h };
            SDL_RenderTexture(r, tLbl, nullptr, &rLbl);
            SDL_DestroyTexture(tLbl);
            SDL_DestroySurface(sLbl);
        }
    } else {
        if (btnCargarTest) {
            btnCargarTest->render(r);
            SDL_Surface* sLbl = TTF_RenderText_Blended(fuenteLista, "Cargar datos de prueba", 0, {0, 100, 0, 255});
            if (sLbl) {
                SDL_Texture* tLbl = SDL_CreateTextureFromSurface(r, sLbl);
                SDL_FRect rLbl = { (w/2.0f) + 30.0f, h - 90.0f, (float)sLbl->w, (float)sLbl->h };
                SDL_RenderTexture(r, tLbl, nullptr, &rLbl);
                SDL_DestroyTexture(tLbl);
                SDL_DestroySurface(sLbl);
            }
        }
    }

    if (btnVolver) btnVolver->render(r);
    
    // CORRECCION: Eliminado SDL_RenderPresent para evitar parpadeo
}
