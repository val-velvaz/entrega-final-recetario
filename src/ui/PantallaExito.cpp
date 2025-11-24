#include "ui/PantallaExito.hpp"
#include "Game.hpp"
#include "persistence/RutasAssets.hpp"
#include "utils/RenderizadorTextos.hpp"
#include <iostream>
#include <cstdlib> // Para rand()

PantallaExito::PantallaExito() 
    : fuenteTitulo(nullptr), fuenteInstruccion(nullptr),
      texTitulo(nullptr), texInstruccion(nullptr) {}

PantallaExito::~PantallaExito() {
    cleanup();
}

void PantallaExito::init(Game& game) {
    SDL_Renderer* renderer = game.getRenderer();
    int w, h;
    SDL_GetRenderOutputSize(renderer, &w, &h);

    fuenteTitulo = TTF_OpenFont(RutasAssets::obtenerRutaFuenteNegrita().c_str(), 48);
    fuenteInstruccion = TTF_OpenFont(RutasAssets::obtenerRutaFuenteRegular().c_str(), 24);

    SDL_Color colorExito = { 46, 139, 87, 255 }; 
    SDL_Color colorGris = { 100, 100, 100, 255 };

    texTitulo = RenderizadorTextos::renderizarTexto(renderer, fuenteTitulo, "RECETA GUARDADA", colorExito);
    texInstruccion = RenderizadorTextos::renderizarTexto(renderer, fuenteInstruccion, "Presione ENTER para continuar", colorGris);

    if (texTitulo) {
        rectTitulo = { (w - (float)texTitulo->w)/2.0f, (h/2.0f) - 50.0f, (float)texTitulo->w, (float)texTitulo->h };
    }
    if (texInstruccion) {
        rectInstruccion = { (w - (float)texInstruccion->w)/2.0f, (h/2.0f) + 20.0f, (float)texInstruccion->w, (float)texInstruccion->h };
    }

    lluvia.clear();
    for (int i = 0; i < 150; ++i) {
        Gota g;
        g.x = (float)(rand() % w);
        g.y = (float)(rand() % h) - h; 
        g.w = (float)(rand() % 5 + 2);
        g.h = (float)(rand() % 40 + 20);
        g.velocidad = (float)(rand() % 8 + 4);
        g.color = paleta[rand() % paleta.size()];
        lluvia.push_back(g);
    }
}

void PantallaExito::cleanup() {
    if (texTitulo) { SDL_DestroyTexture(texTitulo); texTitulo = nullptr; }
    if (texInstruccion) { SDL_DestroyTexture(texInstruccion); texInstruccion = nullptr; }
    if (fuenteTitulo) { TTF_CloseFont(fuenteTitulo); fuenteTitulo = nullptr; }
    if (fuenteInstruccion) { TTF_CloseFont(fuenteInstruccion); fuenteInstruccion = nullptr; }
}

void PantallaExito::handleEvents(Game& game) {
    SDL_Event evento;
    while (SDL_PollEvent(&evento)) {
        if (evento.type == SDL_EVENT_QUIT) {
            game.setEstaCorriendo(false);
        }
        else if (evento.type == SDL_EVENT_KEY_DOWN) {
            if (evento.key.scancode == SDL_SCANCODE_RETURN || evento.key.scancode == SDL_SCANCODE_KP_ENTER) {
                game.popEstado();
            }
        }
        else if (evento.type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
            game.popEstado();
        }
    }
}

void PantallaExito::update(Game& game) {
    (void)game;
    int w, h;
    SDL_GetRenderOutputSize(game.getRenderer(), &w, &h);

    for (auto& g : lluvia) {
        g.y += g.velocidad;
        if (g.y > h) {
            g.y = -(float)(rand() % 100);
            g.x = (float)(rand() % w);
        }
    }
}

void PantallaExito::render(Game& game) {
    SDL_Renderer* r = game.getRenderer();
    int w, h;
    SDL_GetRenderOutputSize(r, &w, &h);

    SDL_SetRenderDrawColor(r, 40, 40, 45, 255);
    SDL_RenderClear(r);

    SDL_SetRenderDrawBlendMode(r, SDL_BLENDMODE_BLEND);
    for (const auto& g : lluvia) {
        SDL_SetRenderDrawColor(r, g.color.r, g.color.g, g.color.b, g.color.a);
        SDL_FRect rectGota = { g.x, g.y, g.w, g.h };
        SDL_RenderFillRect(r, &rectGota);
    }

    SDL_SetRenderDrawColor(r, 255, 255, 255, 230);
    SDL_FRect box = { rectTitulo.x - 40, rectTitulo.y - 30, rectTitulo.w + 80, rectTitulo.h + rectInstruccion.h + 80 };
    SDL_RenderFillRect(r, &box);
    
    SDL_SetRenderDrawColor(r, 200, 200, 200, 255);
    SDL_RenderRect(r, &box);

    SDL_SetRenderDrawBlendMode(r, SDL_BLENDMODE_NONE);

    if (texTitulo) SDL_RenderTexture(r, texTitulo, nullptr, &rectTitulo);
    if (texInstruccion) SDL_RenderTexture(r, texInstruccion, nullptr, &rectInstruccion);

    // CORRECCION: ¡Eliminado SDL_RenderPresent para evitar parpadeo y error!
}
