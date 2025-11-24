#pragma once
#include "ui/GameState.hpp"
#include "ui/Boton.hpp"
#include "ui/CajaDeTexto.hpp"
#include "entities/Categoria.hpp"
#include "entities/Ingrediente.hpp"
#include "entities/Receta.hpp"
#include "datastructures/templates/ListaSimplementeLigada.hpp"
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <vector>
#include <string>

struct ItemIngredienteVisual {
    SDL_Texture* textura;
    SDL_FRect rect;
};

class Game; 

class PantallaAgregarReceta : public GameState {
private:
    TTF_Font* fuenteTitulo;
    TTF_Font* fuenteLabels;
    TTF_Font* fuenteInputs;
    TTF_Font* fuenteLista;
    TTF_Font* fuenteUI;

    SDL_Texture* texturaTitulo;
    SDL_FRect rectTitulo;

    CajaDeTexto* inputNombre;
    CajaDeTexto* inputAutor;
    CajaDeTexto* inputTiempo;
    CajaDeTexto* inputProcedimiento;

    CajaDeTexto* inputIngNombre;
    CajaDeTexto* inputIngCant;
    CajaDeTexto* inputIngUnidad;
    Boton* btnAgregarIngrediente;
    Boton* btnQuitarIngrediente;

    SDL_Texture* texPreviewImagen;
    SDL_FRect rectPreviewImagen;
    std::string rutaImagenTemporal; 
    bool imagenCargada;

    ListaSimplementeLigada<Ingrediente> listaIngredientesTemp;
    std::vector<ItemIngredienteVisual> listaVisualIngredientes;
    void agregarIngredienteALista(SDL_Renderer* renderer);
    void eliminarUltimoIngrediente();

    Categoria categoriaSeleccionada;
    Boton* btnCambiarCategoria;
    SDL_Texture* texturaCategoriaActual;
    SDL_FRect rectCategoria;
    void actualizarTextoCategoria(SDL_Renderer* renderer);

    Boton* btnGuardar;
    Boton* btnCancelar;
    
    bool modoEdicion;
    std::string tempNombre;
    std::string tempAutor;
    std::string tempTiempo;
    std::string tempProc;
    std::string nombreOriginal; // ID para borrar la vieja

    void guardarReceta(Game& game);
    void procesarArchivoDroppeado(SDL_Renderer* renderer, const char* ruta);

public:
    PantallaAgregarReceta();
    PantallaAgregarReceta(const Receta& recetaAEditar);
    virtual ~PantallaAgregarReceta();

    void init(Game& game) override;
    void cleanup() override;
    void handleEvents(Game& game) override;
    void update(Game& game) override;
    void render(Game& game) override;
};
