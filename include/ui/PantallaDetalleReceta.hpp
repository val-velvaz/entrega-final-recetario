#pragma once
#include "ui/GameState.hpp"
#include "ui/Boton.hpp"
#include "entities/Receta.hpp"
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>

class PantallaDetalleReceta : public GameState {
private:
    Receta recetaAMostrar;

    TTF_Font* fuenteTitulo;
    TTF_Font* fuenteSubtitulo;
    TTF_Font* fuenteTexto;

    SDL_Texture* texTitulo;
    SDL_FRect rectTitulo;
    
    // IMAGEN
    SDL_Texture* texImagenPlatillo;
    SDL_FRect rectImagenPlatillo;

    Boton* btnVolver;

    void renderizarInfoGeneral(SDL_Renderer* renderer, int w);
    void renderizarIngredientes(SDL_Renderer* renderer, int w);
    void renderizarProcedimiento(SDL_Renderer* renderer, int w, int h);

public:
    PantallaDetalleReceta(const Receta& receta);
    ~PantallaDetalleReceta();

    void init(Game& game) override;
    void cleanup() override;
    void handleEvents(Game& game) override;
    void update(Game& game) override;
    void render(Game& game) override;
};
