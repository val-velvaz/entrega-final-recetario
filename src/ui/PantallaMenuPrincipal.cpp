#include "ui/PantallaMenuPrincipal.hpp"
#include "ui/PantallaVisualizarRecetas.hpp"
// Si tienes PantallaAgregar, inclúyela: #include "ui/PantallaAgregarReceta.hpp"
#include "persistence/RutasAssets.hpp"
#include <iostream>
#include <stdexcept>

PantallaMenuPrincipal::PantallaMenuPrincipal() 
    : fuenteTitulo(nullptr), texturaTitulo(nullptr), 
      btnAgregar(nullptr), btnVer(nullptr), btnSalir(nullptr) {}

PantallaMenuPrincipal::~PantallaMenuPrincipal() {
    cleanup();
}

void PantallaMenuPrincipal::init(Game& game) {
    SDL_Renderer* renderer = game.getRenderer();
    int ancho, alto;
    SDL_GetRenderOutputSize(renderer, &ancho, &alto);

    // 1. Título
    fuenteTitulo = TTF_OpenFont(RutasAssets::obtenerRutaFuenteNegrita().c_str(), 48);
    if (fuenteTitulo) {
        SDL_Color color = { 80, 80, 80, 255 };
        SDL_Surface* sup = TTF_RenderText_Blended(fuenteTitulo, "RECETARIO DE LA ABUELA", 0, color);
        if (sup) {
            texturaTitulo = SDL_CreateTextureFromSurface(renderer, sup);
            rectTitulo = { (float)(ancho - sup->w)/2.0f, 80.0f, (float)sup->w, (float)sup->h };
            SDL_DestroySurface(sup);
        }
    }

    // 2. Botones (Usando la clase Boton)
    float tam = 100.0f;
    float esp = 50.0f;
    float xIni = (ancho - (tam*3 + esp*2)) / 2.0f;
    float yBtn = 300.0f;

    btnAgregar = new Boton(renderer, RutasAssets::obtenerRutaIconoBotonAgregar(), xIni, yBtn, tam, tam);
    btnVer     = new Boton(renderer, RutasAssets::obtenerRutaIconoBotonGuardar(), xIni + tam + esp, yBtn, tam, tam);
    btnSalir   = new Boton(renderer, RutasAssets::obtenerRutaIconoBotonEliminar(), xIni + (tam + esp)*2, yBtn, tam, tam);
}

void PantallaMenuPrincipal::cleanup() {
    if (texturaTitulo) SDL_DestroyTexture(texturaTitulo);
    if (fuenteTitulo) TTF_CloseFont(fuenteTitulo);
    
    // Los botones se limpian solos al hacer delete
    delete btnAgregar;
    delete btnVer;
    delete btnSalir;
    
    btnAgregar = nullptr;
    btnVer = nullptr;
    btnSalir = nullptr;
}

void PantallaMenuPrincipal::handleEvents(Game& game) {
    SDL_Event evento;
    while (SDL_PollEvent(&evento)) {
        if (evento.type == SDL_EVENT_QUIT) {
            game.setEstaCorriendo(false);
        }
        else if (evento.type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
            if (evento.button.button == SDL_BUTTON_LEFT) {
                float mx = evento.button.x;
                float my = evento.button.y;

                if (btnSalir && btnSalir->estaPresionado(mx, my)) {
                    game.setEstaCorriendo(false);
                }
                else if (btnVer && btnVer->estaPresionado(mx, my)) {
                    game.cambiarEstado(new PantallaVisualizarRecetas());
                }
                else if (btnAgregar && btnAgregar->estaPresionado(mx, my)) {
                   std::cout << "Ir a agregar..." << std::endl;
                   // game.cambiarEstado(new PantallaAgregarReceta());
                }
            }
        }
    }
}

void PantallaMenuPrincipal::update(Game& game) {}

void PantallaMenuPrincipal::render(Game& game) {
    SDL_Renderer* renderer = game.getRenderer();
    if (texturaTitulo) SDL_RenderTexture(renderer, texturaTitulo, nullptr, &rectTitulo);
    
    if (btnAgregar) btnAgregar->render(renderer);
    if (btnVer) btnVer->render(renderer);
    if (btnSalir) btnSalir->render(renderer);
}