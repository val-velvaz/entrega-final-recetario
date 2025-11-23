#include "ui/PantallaMenuPrincipal.hpp"
#include "ui/PantallaVisualizarRecetas.hpp"
#include "ui/PantallaAgregarReceta.hpp"
#include "ui/PantallaSalida.hpp" // <--- NUEVO INCLUDE
#include "Game.hpp"
#include "persistence/RutasAssets.hpp"
#include "utils/RenderizadorTextos.hpp"
#include <iostream>
#include <cmath>

PantallaMenuPrincipal::PantallaMenuPrincipal() 
    : fuenteTitulo(nullptr), texturaTitulo(nullptr), 
      btnAgregar(nullptr), btnVer(nullptr), btnSalir(nullptr), btnHerramientas(nullptr) {}

PantallaMenuPrincipal::~PantallaMenuPrincipal() {
    cleanup();
}

void PantallaMenuPrincipal::init(Game& game) {
    SDL_Renderer* renderer = game.getRenderer();
    int ancho, alto;
    SDL_GetRenderOutputSize(renderer, &ancho, &alto);

    fuenteTitulo = TTF_OpenFont(RutasAssets::obtenerRutaFuenteNegrita().c_str(), 48);
    SDL_Color color = { 80, 80, 80, 255 };
    
    texturaTitulo = RenderizadorTextos::renderizarTexto(renderer, fuenteTitulo, "RECETARIO", color);
    
    if (texturaTitulo) {
        rectTitulo.x = (ancho - texturaTitulo->w)/2.0f;
        rectTitulo.y = 100.0f;
        rectTitulo.w = (float)texturaTitulo->w;
        rectTitulo.h = (float)texturaTitulo->h;
    }

    float tam = 120.0f;
    float esp = 50.0f;
    float xCenter = (ancho / 2.0f) - (tam / 2.0f);
    float yStart = 300.0f;

    btnAgregar = new Boton(renderer, RutasAssets::obtenerRutaIconoBotonAgregar(), xCenter - tam - esp, yStart, tam, tam);
    btnVer = new Boton(renderer, RutasAssets::obtenerRutaIconoBotonGuardar(), xCenter, yStart, tam, tam);
    btnSalir = new Boton(renderer, RutasAssets::obtenerRutaIconoBotonEliminar(), xCenter + tam + esp, yStart, tam, tam);
    btnHerramientas = nullptr; 
}

void PantallaMenuPrincipal::cleanup() {
    if (texturaTitulo) { SDL_DestroyTexture(texturaTitulo); texturaTitulo = nullptr; }
    if (fuenteTitulo) { TTF_CloseFont(fuenteTitulo); fuenteTitulo = nullptr; }
    
    if (btnAgregar) { delete btnAgregar; btnAgregar = nullptr; }
    if (btnVer) { delete btnVer; btnVer = nullptr; }
    if (btnSalir) { delete btnSalir; btnSalir = nullptr; }
}

void PantallaMenuPrincipal::handleEvents(Game& game) {
    SDL_Event evento;
    while (SDL_PollEvent(&evento)) {
        if (evento.type == SDL_EVENT_QUIT) {
            game.setEstaCorriendo(false);
        }
        else if (evento.type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
            if (evento.button.button == SDL_BUTTON_LEFT) {
                float mx = evento.button.x;
                float my = evento.button.y;

                if (btnSalir && btnSalir->estaPresionado(mx, my)) {
                    // CAMBIO: Ir a PantallaSalida en lugar de cerrar directo
                    game.cambiarEstado(new PantallaSalida());
                }
                else if (btnVer && btnVer->estaPresionado(mx, my)) {
                    game.pushEstado(new PantallaVisualizarRecetas());
                }
                else if (btnAgregar && btnAgregar->estaPresionado(mx, my)) {
                   game.pushEstado(new PantallaAgregarReceta());
                }
            }
        }
    }
}

void PantallaMenuPrincipal::update(Game& game) {
    (void)game;
    if (btnAgregar) btnAgregar->actualizar();
    if (btnVer) btnVer->actualizar();
    if (btnSalir) btnSalir->actualizar();

    float tiempo = SDL_GetTicks() / 1000.0f;
    float desplazamiento = std::sin(tiempo * 2.0f) * 10.0f;
    rectTitulo.y = 100.0f + desplazamiento;
}

void PantallaMenuPrincipal::render(Game& game) {
    SDL_Renderer* r = game.getRenderer();
    int w, h;
    SDL_GetRenderOutputSize(r, &w, &h);
    
    // Fondo Rosa Pastel (Mantenemos el diseño anterior)
    SDL_SetRenderDrawColor(r, 255, 228, 235, 255);
    SDL_FRect bg = {0, 0, (float)w, (float)h};
    SDL_RenderFillRect(r, &bg);

    SDL_SetRenderDrawBlendMode(r, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(r, 255, 182, 193, 100);

    float tiempo = SDL_GetTicks() / 50.0f;
    float offset = fmod(tiempo, 100.0f);
    
    for (float i = -100; i < w + h; i += 100) {
        SDL_FRect stripe = { i + offset, 0, 40, (float)h };
        SDL_RenderFillRect(r, &stripe); 
    }
    SDL_SetRenderDrawBlendMode(r, SDL_BLENDMODE_NONE);

    if (texturaTitulo) SDL_RenderTexture(r, texturaTitulo, nullptr, &rectTitulo);
    
    if (btnAgregar) btnAgregar->render(r);
    if (btnVer) btnVer->render(r);
    if (btnSalir) btnSalir->render(r);
}
