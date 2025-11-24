#pragma once
#include "ui/GameState.hpp"
#include "ui/Boton.hpp"
#include "ui/CajaDeTexto.hpp"
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>

class Game; // Forward declaration

class PantallaEliminarReceta : public GameState {
private:
    TTF_Font* fuenteTitulo;
    TTF_Font* fuenteInputs;

    SDL_Texture* texturaTitulo;
    SDL_FRect rectTitulo;

    CajaDeTexto* inputNombre;
    
    Boton* btnEliminar;
    Boton* btnCancelar;

    void eliminarReceta(Game& game);

public:
    PantallaEliminarReceta();
    virtual ~PantallaEliminarReceta();

    void init(Game& game) override;
    void cleanup() override;
    void handleEvents(Game& game) override;
    void update(Game& game) override;
    void render(Game& game) override;
};
