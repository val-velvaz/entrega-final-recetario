#include "ui/PantallaMenuPrincipal.hpp"
#include "ui/PantallaVisualizarRecetas.hpp"
#include "ui/PantallaAgregarReceta.hpp"
#include "ui/PantallaSalida.hpp"
#include "Game.hpp"
#include "persistence/RutasAssets.hpp"
#include "utils/RenderizadorTextos.hpp"
#include <iostream>
#include <cmath>

PantallaMenuPrincipal::PantallaMenuPrincipal() 
    : fuenteTitulo(nullptr), fuenteLabels(nullptr), 
      texturaTitulo(nullptr), texLblAgregar(nullptr), texLblVer(nullptr), texLblSalir(nullptr),
      btnAgregar(nullptr), btnVer(nullptr), btnSalir(nullptr), btnHerramientas(nullptr) {}

PantallaMenuPrincipal::~PantallaMenuPrincipal() {
    cleanup();
}

void PantallaMenuPrincipal::init(Game& game) {
    SDL_Renderer* renderer = game.getRenderer();
    int ancho, alto;
    SDL_GetRenderOutputSize(renderer, &ancho, &alto);

    // Cargar fuentes
    fuenteTitulo = TTF_OpenFont(RutasAssets::obtenerRutaFuenteNegrita().c_str(), 48);
    fuenteLabels = TTF_OpenFont(RutasAssets::obtenerRutaFuenteRegular().c_str(), 20); // Fuente mas chica para etiquetas

    // Titulo Principal
    SDL_Color colorGris = { 80, 80, 80, 255 };
    texturaTitulo = RenderizadorTextos::renderizarTexto(renderer, fuenteTitulo, "RECETARIO", colorGris);
    if (texturaTitulo) {
        rectTitulo.x = (ancho - texturaTitulo->w)/2.0f;
        rectTitulo.y = 100.0f;
        rectTitulo.w = (float)texturaTitulo->w;
        rectTitulo.h = (float)texturaTitulo->h;
    }

    // Configuración de Botones
    float tam = 120.0f; // Tamaño botón
    float esp = 60.0f;  // Espaciado
    float xCenter = (ancho / 2.0f) - (tam / 2.0f);
    float yStart = 300.0f;

    // Crear Botones
    // 1. Agregar (Izquierda)
    btnAgregar = new Boton(renderer, RutasAssets::obtenerRutaIconoBotonAgregar(), xCenter - tam - esp, yStart, tam, tam);
    
    // 2. Ver/Guardar (Centro) - Usamos el icono de 'Guardar' que tenias configurado como Ver
    btnVer = new Boton(renderer, RutasAssets::obtenerRutaIconoBotonGuardar(), xCenter, yStart, tam, tam);
    
    // 3. Salir (Derecha)
    btnSalir = new Boton(renderer, RutasAssets::obtenerRutaIconoBotonEliminar(), xCenter + tam + esp, yStart, tam, tam);
    
    // Crear Etiquetas de Texto (Debajo de los botones)
    SDL_Color colorLabel = { 100, 100, 120, 255 }; // Gris azulado
    
    texLblAgregar = RenderizadorTextos::renderizarTexto(renderer, fuenteLabels, "Nueva Receta", colorLabel);
    texLblVer = RenderizadorTextos::renderizarTexto(renderer, fuenteLabels, "Ver Lista", colorLabel);
    texLblSalir = RenderizadorTextos::renderizarTexto(renderer, fuenteLabels, "Salir", colorLabel);

    // Posicionar Etiquetas (Centradas respecto a su botón)
    float yLabel = yStart + tam + 15.0f;

    if (texLblAgregar) {
        rectLblAgregar.w = (float)texLblAgregar->w;
        rectLblAgregar.h = (float)texLblAgregar->h;
        rectLblAgregar.x = (xCenter - tam - esp) + (tam - rectLblAgregar.w) / 2.0f;
        rectLblAgregar.y = yLabel;
    }

    if (texLblVer) {
        rectLblVer.w = (float)texLblVer->w;
        rectLblVer.h = (float)texLblVer->h;
        rectLblVer.x = xCenter + (tam - rectLblVer.w) / 2.0f;
        rectLblVer.y = yLabel;
    }

    if (texLblSalir) {
        rectLblSalir.w = (float)texLblSalir->w;
        rectLblSalir.h = (float)texLblSalir->h;
        rectLblSalir.x = (xCenter + tam + esp) + (tam - rectLblSalir.w) / 2.0f;
        rectLblSalir.y = yLabel;
    }
}

void PantallaMenuPrincipal::cleanup() {
    if (texturaTitulo) { SDL_DestroyTexture(texturaTitulo); texturaTitulo = nullptr; }
    
    if (texLblAgregar) { SDL_DestroyTexture(texLblAgregar); texLblAgregar = nullptr; }
    if (texLblVer) { SDL_DestroyTexture(texLblVer); texLblVer = nullptr; }
    if (texLblSalir) { SDL_DestroyTexture(texLblSalir); texLblSalir = nullptr; }

    if (fuenteTitulo) { TTF_CloseFont(fuenteTitulo); fuenteTitulo = nullptr; }
    if (fuenteLabels) { TTF_CloseFont(fuenteLabels); fuenteLabels = nullptr; }
    
    if (btnAgregar) { delete btnAgregar; btnAgregar = nullptr; }
    if (btnVer) { delete btnVer; btnVer = nullptr; }
    if (btnSalir) { delete btnSalir; btnSalir = nullptr; }
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
                    game.cambiarEstado(new PantallaSalida());
                }
                else if (btnVer && btnVer->estaPresionado(mx, my)) {
                    game.pushEstado(new PantallaVisualizarRecetas());
                }
                else if (btnAgregar && btnAgregar->estaPresionado(mx, my)) {
                   game.pushEstado(new PantallaAgregarReceta());
                }
            }
        }
    }
}

void PantallaMenuPrincipal::update(Game& game) {
    (void)game;
    if (btnAgregar) btnAgregar->actualizar();
    if (btnVer) btnVer->actualizar();
    if (btnSalir) btnSalir->actualizar();

    // Animación suave del título
    float tiempo = SDL_GetTicks() / 1000.0f;
    float desplazamiento = std::sin(tiempo * 2.0f) * 5.0f;
    rectTitulo.y = 100.0f + desplazamiento;
}

void PantallaMenuPrincipal::render(Game& game) {
    SDL_Renderer* r = game.getRenderer();
    int w, h;
    SDL_GetRenderOutputSize(r, &w, &h);
    
    // Fondo Rosa Pastel
    SDL_SetRenderDrawColor(r, 255, 228, 235, 255);
    SDL_FRect bg = {0, 0, (float)w, (float)h};
    SDL_RenderFillRect(r, &bg);

    // Decoración de fondo (rayas sutiles)
    SDL_SetRenderDrawBlendMode(r, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(r, 255, 182, 193, 100);
    float tiempo = SDL_GetTicks() / 50.0f;
    float offset = fmod(tiempo, 100.0f);
    for (float i = -100; i < w + h; i += 100) {
        SDL_FRect stripe = { i + offset, 0, 40, (float)h };
        SDL_RenderFillRect(r, &stripe); 
    }
    SDL_SetRenderDrawBlendMode(r, SDL_BLENDMODE_NONE);

    // Renderizar Título
    if (texturaTitulo) SDL_RenderTexture(r, texturaTitulo, nullptr, &rectTitulo);
    
    // Renderizar Botones
    if (btnAgregar) btnAgregar->render(r);
    if (btnVer) btnVer->render(r);
    if (btnSalir) btnSalir->render(r);

    // Renderizar Etiquetas (Textos debajo de botones)
    if (texLblAgregar) SDL_RenderTexture(r, texLblAgregar, nullptr, &rectLblAgregar);
    if (texLblVer) SDL_RenderTexture(r, texLblVer, nullptr, &rectLblVer);
    if (texLblSalir) SDL_RenderTexture(r, texLblSalir, nullptr, &rectLblSalir);
}
