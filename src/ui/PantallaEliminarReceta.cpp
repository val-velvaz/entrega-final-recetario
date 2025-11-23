#include "ui/PantallaEliminarReceta.hpp"
#include "ui/PantallaMenuPrincipal.hpp"
#include "Game.hpp"
#include "datastructures/managers/ManejadorRecetas.hpp"
#include "persistence/RutasAssets.hpp"
#include "utils/RenderizadorTextos.hpp"
#include <iostream>
#include <cmath>

PantallaEliminarReceta::PantallaEliminarReceta()
    : fuenteTitulo(nullptr), fuenteInputs(nullptr),
      texturaTitulo(nullptr),
      inputNombre(nullptr), 
      btnEliminar(nullptr), btnCancelar(nullptr) {}

PantallaEliminarReceta::~PantallaEliminarReceta() {
    cleanup();
}

void PantallaEliminarReceta::init(Game& game) {
    SDL_Renderer* renderer = game.getRenderer();
    SDL_Window* window = game.getWindow();
    int w, h;
    SDL_GetRenderOutputSize(renderer, &w, &h);

    fuenteTitulo = TTF_OpenFont(RutasAssets::obtenerRutaFuenteNegrita().c_str(), 32);
    fuenteInputs = TTF_OpenFont(RutasAssets::obtenerRutaFuenteRegular().c_str(), 24);

    SDL_Color colorTitulo = {50, 50, 50, 255};
    texturaTitulo = RenderizadorTextos::renderizarTexto(renderer, fuenteTitulo, "ELIMINAR RECETA", colorTitulo);
    if (texturaTitulo) {
        rectTitulo = { (w - (float)texturaTitulo->w)/2.0f, 50.0f, (float)texturaTitulo->w, (float)texturaTitulo->h };
    }

    float inputW = 400.0f;
    inputNombre = new CajaDeTexto(window, (w - inputW)/2.0f, 200.0f, inputW, 40.0f, fuenteInputs);
    inputNombre->establecerPlaceholder("Nombre exacto a eliminar");

    float btnY = 400.0f;
    btnEliminar = new Boton(renderer, RutasAssets::obtenerRutaIconoBotonEliminar(), w/2.0f - 100.0f, btnY, 80.0f, 80.0f);
    btnCancelar = new Boton(renderer, RutasAssets::obtenerRutaIconoBotonVolver(), w/2.0f + 20.0f, btnY, 80.0f, 80.0f);
}

void PantallaEliminarReceta::cleanup() {
    if (texturaTitulo) { SDL_DestroyTexture(texturaTitulo); texturaTitulo = nullptr; }
    if (fuenteTitulo) { TTF_CloseFont(fuenteTitulo); fuenteTitulo = nullptr; }
    if (fuenteInputs) { TTF_CloseFont(fuenteInputs); fuenteInputs = nullptr; }

    if (inputNombre) { delete inputNombre; inputNombre = nullptr; }
    if (btnEliminar) { delete btnEliminar; btnEliminar = nullptr; }
    if (btnCancelar) { delete btnCancelar; btnCancelar = nullptr; }
}

void PantallaEliminarReceta::handleEvents(Game& game) {
    SDL_Event evento;
    while (SDL_PollEvent(&evento)) {
        if (evento.type == SDL_EVENT_QUIT) {
            game.setEstaCorriendo(false);
        }

        if (inputNombre) inputNombre->manejarEvento(evento);

        if (evento.type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
            if (evento.button.button == SDL_BUTTON_LEFT) {
                float mx = evento.button.x;
                float my = evento.button.y;

                if (btnCancelar && btnCancelar->estaPresionado(mx, my)) {
                    game.popEstado();
                }
                else if (btnEliminar && btnEliminar->estaPresionado(mx, my)) {
                    eliminarReceta(game);
                }
            }
        }
    }
}

void PantallaEliminarReceta::update(Game& game) {
    (void)game;
    if (btnEliminar) btnEliminar->actualizar();
    if (btnCancelar) btnCancelar->actualizar();
}

void PantallaEliminarReceta::render(Game& game) {
    SDL_Renderer* renderer = game.getRenderer();
    int w, h;
    SDL_GetRenderOutputSize(renderer, &w, &h);
    
    // --- FONDO SALMON PASTEL CON PULSO ---
    float tiempo = SDL_GetTicks() / 500.0f;
    Uint8 redIntensity = 245 + (Uint8)(std::sin(tiempo) * 10); // Pulso suave rojizo
    
    SDL_SetRenderDrawColor(renderer, redIntensity, 220, 220, 255);
    SDL_FRect bg = {0, 0, (float)w, (float)h};
    SDL_RenderFillRect(renderer, &bg);

    if (texturaTitulo) SDL_RenderTexture(renderer, texturaTitulo, nullptr, &rectTitulo);
    
    if (inputNombre) inputNombre->render(renderer);

    if (btnEliminar) btnEliminar->render(renderer);
    if (btnCancelar) btnCancelar->render(renderer);
}

void PantallaEliminarReceta::eliminarReceta(Game& game) {
    if (!inputNombre) return;
    std::string nombre = inputNombre->obtenerTexto();
    if (nombre.empty()) return;

    try {
        game.getManejadorRecetas().eliminarRecetaPorNombre(nombre);
        std::cout << "Receta eliminada: " << nombre << std::endl;
        game.popEstado();
    } catch (const std::exception& e) {
        std::cerr << "Error al eliminar: " << e.what() << std::endl;
    }
}
