#include "ui/PantallaBusquedaOrdenamiento.hpp"
#include "Game.hpp"
#include "entities/Receta.hpp"
#include "datastructures/managers/ManejadorRecetas.hpp"
#include "persistence/RutasAssets.hpp"
#include "utils/RenderizadorTextos.hpp"
#include <iostream>

PantallaBusquedaOrdenamiento::PantallaBusquedaOrdenamiento()
    : fuenteTitulo(nullptr), fuenteDetalle(nullptr), texturaTitulo(nullptr),
      btnOrdenarNombre(nullptr), btnOrdenarTiempo(nullptr), btnVolver(nullptr),
      inputBusqueda(nullptr), btnBuscar(nullptr), 
      texturaResultado(nullptr) {
    resultadoTexto = "Selecciona una opcion o busca una receta.";
}

PantallaBusquedaOrdenamiento::~PantallaBusquedaOrdenamiento() {
    cleanup();
}

void PantallaBusquedaOrdenamiento::init(Game& game) {
    SDL_Renderer* renderer = game.getRenderer();
    SDL_Window* window = game.getWindow(); 
    int w, h;
    SDL_GetRenderOutputSize(renderer, &w, &h);

    fuenteTitulo = TTF_OpenFont(RutasAssets::obtenerRutaFuenteNegrita().c_str(), 36);
    fuenteDetalle = TTF_OpenFont(RutasAssets::obtenerRutaFuenteRegular().c_str(), 24);

    texturaTitulo = RenderizadorTextos::renderizarTexto(renderer, fuenteTitulo, "BUSCAR Y ORDENAR", {0,0,0,255});
    if (texturaTitulo) {
        rectTitulo.x = (w - (float)texturaTitulo->w)/2.0f;
        rectTitulo.y = 40.0f;
        rectTitulo.w = (float)texturaTitulo->w;
        rectTitulo.h = (float)texturaTitulo->h;
    }

    float centroX = w / 2.0f;
    float startY = 120.0f;

    btnOrdenarNombre = new Boton(renderer, RutasAssets::obtenerRutaIconoBotonGuardar(), centroX - 120, startY, 80, 80);
    btnOrdenarTiempo = new Boton(renderer, RutasAssets::obtenerRutaIconoBotonEditar(), centroX + 40, startY, 80, 80);

    float searchY = 250.0f;
    inputBusqueda = new CajaDeTexto(window, centroX - 200, searchY, 300, 50, fuenteDetalle);
    inputBusqueda->establecerTexto("");
    inputBusqueda->establecerPlaceholder("Buscar platillo...");

    btnBuscar = new Boton(renderer, RutasAssets::obtenerRutaIconoBotonAgregar(), centroX + 120, searchY, 50, 50);
    btnVolver = new Boton(renderer, RutasAssets::obtenerRutaIconoBotonVolver(), 40, h - 100, 60, 60);

    actualizarTextoResultado(renderer, "Ordena (Arriba) o Busca (Abajo)");
}

// [CORRECCIÓN CRÍTICA] Cleanup seguro contra doble free
void PantallaBusquedaOrdenamiento::cleanup() {
    if (texturaTitulo) { SDL_DestroyTexture(texturaTitulo); texturaTitulo = nullptr; }
    if (texturaResultado) { SDL_DestroyTexture(texturaResultado); texturaResultado = nullptr; }
    
    if (fuenteTitulo) { TTF_CloseFont(fuenteTitulo); fuenteTitulo = nullptr; }
    if (fuenteDetalle) { TTF_CloseFont(fuenteDetalle); fuenteDetalle = nullptr; }

    if (btnOrdenarNombre) { delete btnOrdenarNombre; btnOrdenarNombre = nullptr; }
    if (btnOrdenarTiempo) { delete btnOrdenarTiempo; btnOrdenarTiempo = nullptr; }
    if (btnVolver) { delete btnVolver; btnVolver = nullptr; }
    if (inputBusqueda) { delete inputBusqueda; inputBusqueda = nullptr; }
    if (btnBuscar) { delete btnBuscar; btnBuscar = nullptr; }
}

void PantallaBusquedaOrdenamiento::actualizarTextoResultado(SDL_Renderer* renderer, const std::string& texto) {
    if (texturaResultado) SDL_DestroyTexture(texturaResultado);
    
    texturaResultado = RenderizadorTextos::renderizarTexto(renderer, fuenteDetalle, texto, {0, 50, 150, 255});
    
    if (texturaResultado) {
        int wW, wH;
        SDL_GetRenderOutputSize(renderer, &wW, &wH);
        rectResultado.x = (wW - (float)texturaResultado->w)/2.0f;
        rectResultado.y = 350.0f;
        rectResultado.w = (float)texturaResultado->w;
        rectResultado.h = (float)texturaResultado->h;
    }
}

void PantallaBusquedaOrdenamiento::handleEvents(Game& game) {
    SDL_Event ev;
    while (SDL_PollEvent(&ev)) {
        if (ev.type == SDL_EVENT_QUIT) game.setEstaCorriendo(false);

        if (inputBusqueda) inputBusqueda->manejarEvento(ev);

        if (ev.type == SDL_EVENT_MOUSE_BUTTON_DOWN && ev.button.button == SDL_BUTTON_LEFT) {
            float mx = ev.button.x;
            float my = ev.button.y;

            if (btnVolver && btnVolver->estaPresionado(mx, my)) {
                game.popEstado();
            }
            else if (btnOrdenarNombre && btnOrdenarNombre->estaPresionado(mx, my)) {
                game.getManejadorRecetas().ordenarRecetasPorNombre();
                actualizarTextoResultado(game.getRenderer(), "Recetas ordenadas alfabeticamente.");
            }
            else if (btnOrdenarTiempo && btnOrdenarTiempo->estaPresionado(mx, my)) {
                game.getManejadorRecetas().ordenarRecetasPorTiempoPreparacion();
                actualizarTextoResultado(game.getRenderer(), "Recetas ordenadas por tiempo.");
            }
            else if (btnBuscar && btnBuscar->estaPresionado(mx, my)) {
                std::string query = inputBusqueda->obtenerTexto();
                if (query.empty()) {
                    actualizarTextoResultado(game.getRenderer(), "Escribe un nombre primero.");
                } else {
                    Receta* encontrado = game.getManejadorRecetas().buscarRecetaBinariaPorNombre(query);
                    if (encontrado) {
                        std::string msg = "ENCONTRADO: " + encontrado->obtenerNombrePlatillo() + 
                                          " (" + std::to_string(encontrado->obtenerTiempoPreparacion()) + " min)";
                        actualizarTextoResultado(game.getRenderer(), msg);
                    } else {
                        actualizarTextoResultado(game.getRenderer(), "No encontrado.");
                    }
                }
            }
        }
    }
}

void PantallaBusquedaOrdenamiento::update(Game& game) {
    (void)game;
    if (btnOrdenarNombre) btnOrdenarNombre->actualizar();
    if (btnOrdenarTiempo) btnOrdenarTiempo->actualizar();
    if (btnBuscar) btnBuscar->actualizar();
    if (btnVolver) btnVolver->actualizar();
}

void PantallaBusquedaOrdenamiento::render(Game& game) {
    SDL_Renderer* r = game.getRenderer();
    
    SDL_SetRenderDrawColor(r, 240, 240, 245, 255);
    SDL_FRect bg = {0,0,1024,768};
    SDL_RenderFillRect(r, &bg);

    if (texturaTitulo) SDL_RenderTexture(r, texturaTitulo, nullptr, &rectTitulo);

    if (btnOrdenarNombre) btnOrdenarNombre->render(r);
    if (btnOrdenarTiempo) btnOrdenarTiempo->render(r);
    if (btnBuscar) btnBuscar->render(r);
    if (btnVolver) btnVolver->render(r);
    
    if (inputBusqueda) inputBusqueda->render(r);

    if (texturaResultado) SDL_RenderTexture(r, texturaResultado, nullptr, &rectResultado);
}
