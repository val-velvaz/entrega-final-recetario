#include "ui/Boton.hpp"
#include <SDL3_image/SDL_image.h>
#include <iostream>

Boton::Boton(SDL_Renderer* renderer, const std::string& rutaImagen, float x, float y, float w, float h) 
    : textura(nullptr), activo(true), hover(false) {
    
    rectOriginal = {x, y, w, h};
    rectActual = rectOriginal;

    textura = IMG_LoadTexture(renderer, rutaImagen.c_str());
}

Boton::~Boton() {
    if (textura) {
        SDL_DestroyTexture(textura);
    }
}

// NUEVO MÉTODO IMPLEMENTADO
void Boton::cambiarImagen(SDL_Renderer* renderer, const std::string& nuevaRuta) {
    if (textura) {
        SDL_DestroyTexture(textura); // Borramos la vieja para no llenar la memoria
    }
    textura = IMG_LoadTexture(renderer, nuevaRuta.c_str());
    
    if (!textura) {
        // std::cerr << "[WARN] No se pudo cargar nueva imagen boton: " << nuevaRuta << std::endl;
    }
}

void Boton::actualizar() {
    if (!activo) return;

    float mx, my;
    SDL_GetMouseState(&mx, &my);

    bool colision = (mx >= rectOriginal.x && mx <= (rectOriginal.x + rectOriginal.w) &&
                     my >= rectOriginal.y && my <= (rectOriginal.y + rectOriginal.h));

    hover = colision;

    float targetScale = hover ? 1.1f : 1.0f;
    float targetW = rectOriginal.w * targetScale;
    float targetH = rectOriginal.h * targetScale;
    
    float speed = 0.2f; 
    rectActual.w += (targetW - rectActual.w) * speed;
    rectActual.h += (targetH - rectActual.h) * speed;

    rectActual.x = rectOriginal.x - (rectActual.w - rectOriginal.w) / 2.0f;
    rectActual.y = rectOriginal.y - (rectActual.h - rectOriginal.h) / 2.0f;
}

void Boton::render(SDL_Renderer* renderer) {
    if (!activo) return;

    if (textura) {
        SDL_RenderTexture(renderer, textura, nullptr, &rectActual);
    } else {
        if (hover) SDL_SetRenderDrawColor(renderer, 255, 150, 150, 255);
        else SDL_SetRenderDrawColor(renderer, 200, 50, 50, 255);
        SDL_RenderRect(renderer, &rectActual);
    }
}

bool Boton::estaPresionado(float mouseX, float mouseY) const {
    if (!activo) return false;
    return (mouseX >= rectOriginal.x && mouseX <= (rectOriginal.x + rectOriginal.w) &&
            mouseY >= rectOriginal.y && mouseY <= (rectOriginal.y + rectOriginal.h));
}
