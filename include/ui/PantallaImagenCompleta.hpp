#pragma once
#include "ui/GameState.hpp"
#include <SDL3/SDL.h>
#include <string>

class PantallaImagenCompleta : public GameState {
private:
    std::string rutaImagen;
    SDL_Texture* texturaImagen;
    SDL_FRect rectImagen;

public:
    PantallaImagenCompleta(const std::string& ruta);
    ~PantallaImagenCompleta();

    void init(Game& game) override;
    void cleanup() override;
    void handleEvents(Game& game) override;
    void update(Game& game) override;
    void render(Game& game) override;
};
