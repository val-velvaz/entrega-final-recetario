#pragma once
#include "ui/GameState.hpp"
#include "ui/Boton.hpp"
#include "ui/CajaDeTexto.hpp"
#include "ui/PantallaGestionArchivos.hpp"
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <vector>
#include <string>

struct ItemLista {
    SDL_Texture* textura;
    SDL_Texture* texMiniatura;
    SDL_FRect rect;
    int indiceOriginal;
    SDL_FRect rectOjo;
    SDL_FRect rectEditar;
    SDL_FRect rectBasura;
};

class PantallaVisualizarRecetas : public GameState {
private:
    TTF_Font* fuenteTitulo;
    TTF_Font* fuenteLista;
    TTF_Font* fuenteUI; 
    
    SDL_Texture* texturaTitulo;
    SDL_FRect rectTitulo;
    
    CajaDeTexto* inputBusqueda;
    Boton* btnBuscar;
    Boton* btnReset;

    Boton* btnOrdNombre;
    Boton* btnOrdTiempo;
    Boton* btnOrdCategoria;
    Boton* btnOrdAutor;
    
    Boton* btnCargar;
    Boton* btnGuardar;
    Boton* btnEliminarTodo;
    
    SDL_Texture* texIconoOjo;
    SDL_Texture* texIconoBasura;
    SDL_Texture* texIconoEditar;

    SDL_Texture* texLblNombre; SDL_FRect rLblNombre;
    SDL_Texture* texLblTiempo; SDL_FRect rLblTiempo;
    SDL_Texture* texLblCat;    SDL_FRect rLblCat;
    SDL_Texture* texLblAut;    SDL_FRect rLblAut;
    SDL_Texture* texLblCargar; SDL_FRect rLblCargar;
    SDL_Texture* texLblGuardar; SDL_FRect rLblGuardar;
    SDL_Texture* texLblDelAll; SDL_FRect rLblDelAll;

    Boton* btnVolver;
    std::vector<ItemLista> itemsLista;
    std::string filtroNombre; 

    // CORRECCIÓN: Variable para detectar cambios en los datos y auto-actualizarse
    int cacheCantidadRecetas;

    void generarCacheListado(Game& game);

public:
    PantallaVisualizarRecetas();
    ~PantallaVisualizarRecetas();

    void init(Game& game) override;
    void cleanup() override;
    void handleEvents(Game& game) override;
    void update(Game& game) override;
    void render(Game& game) override;
};
