#pragma once
#include "ui/GameState.hpp"
#include "ui/Boton.hpp"
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>

class PantallaMenuPrincipal : public GameState {
private:
    TTF_Font* fuenteTitulo;
    SDL_Texture* texturaTitulo;
    SDL_FRect rectTitulo;

    Boton* btnAgregar;
    Boton* btnVer;
    Boton* btnSalir;
    Boton* btnHerramientas;

public:
    PantallaMenuPrincipal();
    ~PantallaMenuPrincipal();

    void init(Game& game) override;
    void cleanup() override;

    void handleEvents(Game& game) override;
    void update(Game& game) override;
    void render(Game& game) override;
};
