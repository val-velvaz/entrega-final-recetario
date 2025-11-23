#pragma once
#include "ui/GameState.hpp"
#include "ui/Boton.hpp"
#include "ui/CajaDeTexto.hpp"
#include <vector>

class PantallaBusquedaOrdenamiento : public GameState {
private:
    TTF_Font* fuenteTitulo;
    TTF_Font* fuenteDetalle;
    SDL_Texture* texturaTitulo;
    SDL_FRect rectTitulo;

    // Botones de Ordenamiento
    Boton* btnOrdenarNombre;
    Boton* btnOrdenarTiempo;
    Boton* btnVolver;

    // Búsqueda
    CajaDeTexto* inputBusqueda;
    Boton* btnBuscar;

    // Resultado
    std::string resultadoTexto;
    SDL_Texture* texturaResultado;
    SDL_FRect rectResultado;

    void actualizarTextoResultado(SDL_Renderer* renderer, const std::string& texto);

public:
    PantallaBusquedaOrdenamiento();
    ~PantallaBusquedaOrdenamiento();

    void init(Game& game) override;
    void cleanup() override;
    void handleEvents(Game& game) override;
    void update(Game& game) override;
    void render(Game& game) override;
};
