#pragma once
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <string>

class RenderizadorTextos {
public:
    static SDL_Texture* renderizarTexto(SDL_Renderer* renderer, TTF_Font* fuente, const std::string& texto, SDL_Color color);
};
