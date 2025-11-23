#include "ui/PantallaImagenCompleta.hpp"
#include "Game.hpp"
#include <SDL3_image/SDL_image.h>
#include <algorithm>

PantallaImagenCompleta::PantallaImagenCompleta(const std::string& ruta) 
    : rutaImagen(ruta), texturaImagen(nullptr) {}

PantallaImagenCompleta::~PantallaImagenCompleta() {
    cleanup();
}

void PantallaImagenCompleta::init(Game& game) {
    SDL_Renderer* renderer = game.getRenderer();
    int w, h;
    SDL_GetRenderOutputSize(renderer, &w, &h);

    texturaImagen = IMG_LoadTexture(renderer, rutaImagen.c_str());
    if (texturaImagen) {
        float imgW = (float)texturaImagen->w;
        float imgH = (float)texturaImagen->h;
        
        // Ajustar para que quepa en pantalla manteniendo proporcion (Aspect Fit)
        float scale = std::min((float)w / imgW, (float)h / imgH);
        float finalW = imgW * scale;
        float finalH = imgH * scale;

        rectImagen.x = (w - finalW) / 2.0f;
        rectImagen.y = (h - finalH) / 2.0f;
        rectImagen.w = finalW;
        rectImagen.h = finalH;
    }
}

void PantallaImagenCompleta::cleanup() {
    if (texturaImagen) { SDL_DestroyTexture(texturaImagen); texturaImagen = nullptr; }
}

void PantallaImagenCompleta::handleEvents(Game& game) {
    SDL_Event evento;
    while (SDL_PollEvent(&evento)) {
        if (evento.type == SDL_EVENT_QUIT) {
            game.setEstaCorriendo(false);
        }
        // Cualquier click o tecla cierra la imagen
        else if (evento.type == SDL_EVENT_MOUSE_BUTTON_DOWN || 
                 evento.type == SDL_EVENT_KEY_DOWN) {
            game.popEstado();
        }
    }
}

void PantallaImagenCompleta::update(Game& game) { (void)game; }

void PantallaImagenCompleta::render(Game& game) {
    SDL_Renderer* r = game.getRenderer();
    
    // Fondo negro semitransparente para dimming
    SDL_SetRenderDrawColor(r, 0, 0, 0, 240);
    SDL_FRect bg = {0,0,10000,10000};
    SDL_RenderFillRect(r, &bg);

    if (texturaImagen) {
        SDL_RenderTexture(r, texturaImagen, nullptr, &rectImagen);
    }
    
    SDL_RenderPresent(r);
}
