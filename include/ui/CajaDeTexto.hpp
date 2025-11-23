#pragma once
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <string>

class CajaDeTexto {
private:
    SDL_Window* ventana; // Referencia a la ventana para activar inputs
    SDL_FRect rect;
    std::string texto;
    std::string placeholder;
    bool tieneFoco;
    TTF_Font* fuente;
    SDL_Texture* texturaTexto;
    
    SDL_Color colorFondo = {255, 255, 255, 255};
    SDL_Color colorBorde = {0, 0, 0, 255};
    SDL_Color colorTexto = {0, 0, 0, 255};
    SDL_Color colorPlaceholder = {150, 150, 150, 255};

    void regenerarTextura(SDL_Renderer* renderer);

public:
    // Constructor actualizado con SDL_Window*
    CajaDeTexto(SDL_Window* win, float x, float y, float w, float h, TTF_Font* f);
    ~CajaDeTexto();

    void manejarEvento(const SDL_Event& evento);
    void render(SDL_Renderer* renderer);
    
    std::string obtenerTexto() const { return texto; }
    void establecerTexto(const std::string& t);
    void establecerPlaceholder(const std::string& p) { placeholder = p; }
    
    bool estaActiva() const { return tieneFoco; }
};
