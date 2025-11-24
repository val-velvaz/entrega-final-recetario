#pragma once
#include "ui/GameState.hpp"
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <vector>
#include <string>

class PantallaExito : public GameState {
private:
    TTF_Font* fuenteTitulo;
    TTF_Font* fuenteInstruccion;

    SDL_Texture* texTitulo;
    SDL_Texture* texInstruccion;

    SDL_FRect rectTitulo;
    SDL_FRect rectInstruccion;

    // Sistema de Partículas (Estilo Lluvia)
    struct Gota {
        float x, y;
        float w, h;
        float velocidad;
        SDL_Color color;
    };
    
    std::vector<Gota> lluvia;
    
    // Paleta de colores pastel
    const std::vector<SDL_Color> paleta = {
        {255, 182, 193, 200}, // Rosa
        {173, 216, 230, 200}, // Azul Claro
        {255, 253, 208, 200}, // Crema
        {152, 251, 152, 200}, // Verde Palido
        {221, 160, 221, 200}  // Ciruela
    };

public:
    PantallaExito();
    ~PantallaExito();

    void init(Game& game) override;
    void cleanup() override;
    void handleEvents(Game& game) override;
    void update(Game& game) override;
    void render(Game& game) override;
};
