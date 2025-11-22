#pragma once
#include "ui/GameState.hpp"
#include "entities/Receta.hpp"
#include "ui/Boton.hpp" // Asumo que tienes este header o el .o
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>

class PantallaDetalleReceta : public GameState {
private:
    Receta recetaActual; // Copia local o referencia segura
    
    TTF_Font* fuenteTitulo;
    TTF_Font* fuenteTexto;
    
    SDL_Texture* texTitulo;
    SDL_Texture* texIngredientes;
    SDL_Texture* texProcedimiento;
    SDL_Texture* texImagenPlatillo; // La foto

    SDL_FRect rectTitulo;
    SDL_FRect rectContenido;
    SDL_FRect rectImagen;

    // Botón para volver
    SDL_Texture* texBotonVolver;
    SDL_FRect rectBotonVolver;

    void generarTexturasTexto(SDL_Renderer* renderer);

public:
    // Constructor que recibe la receta a mostrar
    PantallaDetalleReceta(const Receta& receta);
    ~PantallaDetalleReceta();

    void init(Game& game) override;
    void cleanup() override;
    void handleEvents(Game& game) override;
    void update(Game& game) override;
    void render(Game& game) override;
};