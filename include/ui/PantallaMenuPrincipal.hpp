#pragma once
#include "ui/GameState.hpp"
#include "Game.hpp"
#include "ui/Boton.hpp" // Importante

class PantallaMenuPrincipal : public GameState {
private:
    TTF_Font* fuenteTitulo;
    SDL_Texture* texturaTitulo;
    SDL_FRect rectTitulo;

    // Botones encapsulados
    Boton* btnAgregar;
    Boton* btnVer;
    Boton* btnSalir;

public:
    PantallaMenuPrincipal();
    virtual ~PantallaMenuPrincipal();

    virtual void init(Game& game) override;
    virtual void cleanup() override;
    virtual void handleEvents(Game& game) override;
    virtual void update(Game& game) override;
    virtual void render(Game& game) override;
};