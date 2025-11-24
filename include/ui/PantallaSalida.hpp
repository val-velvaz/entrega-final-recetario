#pragma once
#include "ui/GameState.hpp"
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <string>

class PantallaSalida : public GameState {
private:
    TTF_Font* fuente;
    SDL_Texture* textoBase;
    SDL_FRect rectTexto;
    
    float tiempoAcumulado;
    int puntos; // 0, 1, 2, 3
    int ciclosCompletados;
    Uint64 ultimoTick;

public:
    PantallaSalida();
    ~PantallaSalida();

    void init(Game& game) override;
    void cleanup() override;
    void handleEvents(Game& game) override;
    void update(Game& game) override;
    void render(Game& game) override;
};
