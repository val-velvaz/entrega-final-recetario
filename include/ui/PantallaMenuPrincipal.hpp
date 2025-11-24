#pragma once
#include "ui/GameState.hpp"
#include "ui/Boton.hpp"
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>

class PantallaMenuPrincipal : public GameState {
private:
    TTF_Font* fuenteTitulo;
    TTF_Font* fuenteLabels; // Fuente para los textos de abajo

    SDL_Texture* texturaTitulo;
    SDL_FRect rectTitulo;

    // Texturas de las etiquetas
    SDL_Texture* texLblAgregar;
    SDL_Texture* texLblVer;
    SDL_Texture* texLblSalir;

    // Posiciones de las etiquetas
    SDL_FRect rectLblAgregar;
    SDL_FRect rectLblVer;
    SDL_FRect rectLblSalir;

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
