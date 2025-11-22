#include "ui/Boton.hpp"

Boton::Boton(SDL_Renderer* renderer, const std::string& rutaImagen, float x, float y, float w, float h) 
    : textura(nullptr) {
    
    rectangulo = { x, y, w, h };

    textura = IMG_LoadTexture(renderer, rutaImagen.c_str());
    if (!textura) {
        std::cerr << "[ERROR UI] No se pudo cargar boton: " << rutaImagen 
                  << " | " << SDL_GetError() << std::endl;
    }
}

Boton::~Boton() {
    if (textura) {
        SDL_DestroyTexture(textura);
        textura = nullptr;
    }
}

void Boton::render(SDL_Renderer* renderer) {
    if (textura) {
        SDL_RenderTexture(renderer, textura, nullptr, &rectangulo);
    }
}

bool Boton::estaPresionado(float mouseX, float mouseY) const {
    return (mouseX >= rectangulo.x && mouseX <= (rectangulo.x + rectangulo.w) &&
            mouseY >= rectangulo.y && mouseY <= (rectangulo.y + rectangulo.h));
}