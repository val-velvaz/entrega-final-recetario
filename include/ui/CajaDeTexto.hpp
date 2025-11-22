#pragma once
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <string>

class CajaDeTexto {
private:
    SDL_FRect rect;
    std::string texto;
    bool tieneFoco;
    TTF_Font* fuente;
    SDL_Texture* texturaTexto;
    
    // Configuración visual
    SDL_Color colorFondo = {255, 255, 255, 255};
    SDL_Color colorBorde = {0, 0, 0, 255};
    SDL_Color colorTexto = {0, 0, 0, 255};

    void regenerarTextura(SDL_Renderer* renderer);

public:
    CajaDeTexto(float x, float y, float w, float h, TTF_Font* f);
    ~CajaDeTexto();

    void manejarEventos(const SDL_Event& evento);
    void renderizar(SDL_Renderer* renderer);
    
    std::string obtenerTexto() const { return texto; }
    void establecerTexto(const std::string& t) { texto = t; }
    bool estaActiva() const { return tieneFoco; }
};