#include "utils/RenderizadorTextos.hpp"
#include <iostream>

SDL_Texture* RenderizadorTextos::renderizarTexto(SDL_Renderer* renderer, TTF_Font* fuente, const std::string& texto, SDL_Color color) {
    // Validaciones básicas
    if (!renderer) {
        // std::cerr << "[WARN] Renderizador nulo en renderizarTexto" << std::endl;
        return nullptr;
    }
    if (!fuente) {
        // std::cerr << "[WARN] Fuente nula en renderizarTexto para: " << texto << std::endl;
        return nullptr;
    }
    if (texto.empty()) {
        return nullptr;
    }

    // Renderizar superficie (0 = longitud automática hasta null-terminator)
    SDL_Surface* superficie = TTF_RenderText_Blended(fuente, texto.c_str(), 0, color);
    
    if (!superficie) {
        // std::cerr << "[ERROR] Fallo TTF_RenderText: " << SDL_GetError() << std::endl;
        return nullptr;
    }

    // Crear textura desde superficie
    SDL_Texture* textura = SDL_CreateTextureFromSurface(renderer, superficie);
    
    // Liberar superficie temporal
    SDL_DestroySurface(superficie);

    if (!textura) {
        std::cerr << "[ERROR] Fallo SDL_CreateTextureFromSurface: " << SDL_GetError() << std::endl;
    }

    return textura;
}
