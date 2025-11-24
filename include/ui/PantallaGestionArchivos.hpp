#pragma once
#include "ui/GameState.hpp"
#include "ui/Boton.hpp"
#include "ui/CajaDeTexto.hpp"
#include <vector>
#include <string>
#include <filesystem>
#include <future>
#include <chrono>
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>

class PantallaGestionArchivos : public GameState {
private:
    TTF_Font* fuenteTitulo;
    TTF_Font* fuenteLista;
    TTF_Font* fuenteSettings;
    TTF_Font* fuenteCargando;
    SDL_Texture* texTitulo;
    SDL_FRect rectTitulo;

    bool modoGuardar;

    struct BotonArchivo {
        Boton* btn;
        SDL_Texture* label;
        SDL_FRect rectLabel;
        std::string nombreArchivo;
    };
    std::vector<BotonArchivo> listaBotonesArchivos;

    Boton* btnNuevoArchivo; 
    CajaDeTexto* inputNuevoArchivo;
    Boton* btnCargarTest;
    Boton* btnVolver;
    
    Boton* btnSeparador;
    SDL_Texture* texSeparador;
    std::string separadorActual;

    // Estado de carga asíncrona
    bool cargandoDatos;
    std::future<void> tareaIO;
    std::string rutaPendiente;
    bool modoGuardarPendiente;
    SDL_Texture* texCargando;
    SDL_FRect rectCargando;
    
    // Variable para controlar el debounce (evitar doble clic)
    Uint32 tiempoInicio; 

    void escanearDirectorio(SDL_Renderer* renderer);
    void seleccionarArchivo(Game& game, const std::string& nombre);
    void crearYSeleccionar(Game& game);
    void cargarDatosPrueba(Game& game);
    void actualizarSeparador(SDL_Renderer* renderer);

public:
    PantallaGestionArchivos(bool esModoGuardar);
    ~PantallaGestionArchivos();

    void init(Game& game) override;
    void cleanup() override;
    void handleEvents(Game& game) override;
    void update(Game& game) override;
    void render(Game& game) override;
};
