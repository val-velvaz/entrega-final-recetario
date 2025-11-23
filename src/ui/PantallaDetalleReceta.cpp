#include "ui/PantallaDetalleReceta.hpp"
#include "ui/PantallaImagenCompleta.hpp"
#include "Game.hpp"
#include "persistence/RutasAssets.hpp"
#include "utils/RenderizadorTextos.hpp"
#include "entities/Categoria.hpp"
#include <iostream>
#include <SDL3_image/SDL_image.h>

PantallaDetalleReceta::PantallaDetalleReceta(const Receta& receta)
    : recetaAMostrar(receta), 
      fuenteTitulo(nullptr), fuenteSubtitulo(nullptr), fuenteTexto(nullptr),
      texTitulo(nullptr), rectTitulo({0,0,0,0}), 
      texImagenPlatillo(nullptr), rectImagenPlatillo({0,0,0,0}),
      btnVolver(nullptr) {}

PantallaDetalleReceta::~PantallaDetalleReceta() {
    cleanup();
}

void PantallaDetalleReceta::init(Game& game) {
    SDL_Renderer* renderer = game.getRenderer();
    int w, h;
    SDL_GetRenderOutputSize(renderer, &w, &h);

    fuenteTitulo = TTF_OpenFont(RutasAssets::obtenerRutaFuenteNegrita().c_str(), 32);
    fuenteSubtitulo = TTF_OpenFont(RutasAssets::obtenerRutaFuenteNegrita().c_str(), 20);
    fuenteTexto = TTF_OpenFont(RutasAssets::obtenerRutaFuenteRegular().c_str(), 18);

    texTitulo = RenderizadorTextos::renderizarTexto(renderer, fuenteTitulo, recetaAMostrar.obtenerNombrePlatillo(), {50, 50, 70, 255});
    if (texTitulo) {
        rectTitulo = { 50.0f, 30.0f, (float)texTitulo->w, (float)texTitulo->h };
    }

    std::string rutaImg = recetaAMostrar.obtenerRutaImagen();
    if (!rutaImg.empty()) {
        texImagenPlatillo = IMG_LoadTexture(renderer, rutaImg.c_str());
    }
    if (!texImagenPlatillo) {
        std::string defaultPath = RutasAssets::obtenerRutaImagenPlatillo(recetaAMostrar.obtenerCategoria(), "default");
        texImagenPlatillo = IMG_LoadTexture(renderer, defaultPath.c_str());
    }
    
    if (texImagenPlatillo) {
        rectImagenPlatillo = { (float)w - 350.0f, 100.0f, 300.0f, 300.0f }; 
    }

    btnVolver = new Boton(renderer, RutasAssets::obtenerRutaIconoBotonVolver(), 50, h - 80, 60, 60);
}

// [CORREGIDO] Cleanup seguro: Asignar nullptr para evitar doble free
void PantallaDetalleReceta::cleanup() {
    if (texTitulo) { SDL_DestroyTexture(texTitulo); texTitulo = nullptr; }
    if (texImagenPlatillo) { SDL_DestroyTexture(texImagenPlatillo); texImagenPlatillo = nullptr; }
    
    if (fuenteTitulo) { TTF_CloseFont(fuenteTitulo); fuenteTitulo = nullptr; }
    if (fuenteSubtitulo) { TTF_CloseFont(fuenteSubtitulo); fuenteSubtitulo = nullptr; }
    if (fuenteTexto) { TTF_CloseFont(fuenteTexto); fuenteTexto = nullptr; }
    
    if (btnVolver) { delete btnVolver; btnVolver = nullptr; }
}

void PantallaDetalleReceta::handleEvents(Game& game) {
    SDL_Event evento;
    while (SDL_PollEvent(&evento)) {
        if (evento.type == SDL_EVENT_QUIT) game.setEstaCorriendo(false);

        if (evento.type == SDL_EVENT_MOUSE_BUTTON_DOWN && evento.button.button == SDL_BUTTON_LEFT) {
            float mx = evento.button.x;
            float my = evento.button.y;
            if (btnVolver && btnVolver->estaPresionado(mx, my)) {
                game.popEstado();
            }
            
            if (texImagenPlatillo && 
                mx >= rectImagenPlatillo.x && mx <= rectImagenPlatillo.x + rectImagenPlatillo.w &&
                my >= rectImagenPlatillo.y && my <= rectImagenPlatillo.y + rectImagenPlatillo.h) {
                
                std::string ruta = recetaAMostrar.obtenerRutaImagen();
                if (ruta.empty()) ruta = RutasAssets::obtenerRutaImagenPlatillo(recetaAMostrar.obtenerCategoria(), "default");
                game.pushEstado(new PantallaImagenCompleta(ruta));
            }
        }
    }
}

void PantallaDetalleReceta::update(Game& game) {
    (void)game;
    if (btnVolver) btnVolver->actualizar();
}

void PantallaDetalleReceta::render(Game& game) {
    SDL_Renderer* renderer = game.getRenderer();
    int w, h;
    SDL_GetRenderOutputSize(renderer, &w, &h);

    SDL_SetRenderDrawColor(renderer, 250, 248, 240, 255); 
    SDL_FRect bg = {0,0,1024,768};
    SDL_RenderFillRect(renderer, &bg);

    if (texTitulo) SDL_RenderTexture(renderer, texTitulo, nullptr, &rectTitulo);

    renderizarInfoGeneral(renderer, w);
    renderizarIngredientes(renderer, w);
    renderizarProcedimiento(renderer, w, h);

    if (texImagenPlatillo) {
        SDL_RenderTexture(renderer, texImagenPlatillo, nullptr, &rectImagenPlatillo);
        SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
        SDL_RenderRect(renderer, &rectImagenPlatillo);
    }

    if (btnVolver) btnVolver->render(renderer);
}

void PantallaDetalleReceta::renderizarInfoGeneral(SDL_Renderer* renderer, int w) {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_FRect card = {50, 80, (float)w-420, 60}; 
    SDL_RenderFillRect(renderer, &card);

    std::string info = "Autor: " + recetaAMostrar.obtenerAutor().obtenerNombreCompleto() + 
                       "  |  Tiempo: " + std::to_string(recetaAMostrar.obtenerTiempoPreparacion()) + " min" +
                       "  |  Categoria: " + categoriaATexto(recetaAMostrar.obtenerCategoria());
    
    SDL_Surface* s = TTF_RenderText_Blended(fuenteTexto, info.c_str(), 0, {80, 80, 80, 255});
    if (s) {
        SDL_Texture* t = SDL_CreateTextureFromSurface(renderer, s);
        if (t) {
            SDL_FRect r = { card.x + 20, card.y + 15, (float)s->w, (float)s->h };
            SDL_RenderTexture(renderer, t, nullptr, &r);
            SDL_DestroyTexture(t);
        }
        SDL_DestroySurface(s);
    }
}

void PantallaDetalleReceta::renderizarIngredientes(SDL_Renderer* renderer, int w) {
    (void)w;
    float yStart = 160.0f;
    
    SDL_Surface* sSub = TTF_RenderText_Blended(fuenteSubtitulo, "INGREDIENTES:", 0, {0, 0, 0, 255});
    if(sSub) {
        SDL_Texture* t = SDL_CreateTextureFromSurface(renderer, sSub);
        SDL_FRect r = { 60, yStart, (float)sSub->w, (float)sSub->h };
        SDL_RenderTexture(renderer, t, nullptr, &r);
        SDL_DestroyTexture(t);
        SDL_DestroySurface(sSub);
    }

    const auto& listaIng = recetaAMostrar.obtenerIngredientesConstante();
    float yIng = yStart + 30.0f;
    
    for (int i = 0; i < listaIng.obtenerCantidadElementos(); ++i) {
        SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
        SDL_FRect bullet = { 60.0f, yIng + 8, 5, 5 };
        SDL_RenderFillRect(renderer, &bullet);
        
        try {
            const Ingrediente& ing = listaIng.obtenerEnPosicion(i);
            std::string textoIng = ing.obtenerNombre() + " (" + ing.obtenerCantidad() + " " + ing.obtenerUnidad() + ")";
            
            SDL_Surface* sIng = TTF_RenderText_Blended(fuenteTexto, textoIng.c_str(), 0, {60, 60, 60, 255});
            if (sIng) {
                SDL_Texture* tIng = SDL_CreateTextureFromSurface(renderer, sIng);
                SDL_FRect rIng = { 75.0f, yIng, (float)sIng->w, (float)sIng->h };
                SDL_RenderTexture(renderer, tIng, nullptr, &rIng);
                SDL_DestroyTexture(tIng);
                SDL_DestroySurface(sIng);
            }
        } catch (...) {}

        yIng += 25.0f;
    }
}

void PantallaDetalleReceta::renderizarProcedimiento(SDL_Renderer* renderer, int w, int h) {
    float yStart = 400.0f;
    
    SDL_Surface* sSub = TTF_RenderText_Blended(fuenteSubtitulo, "PROCEDIMIENTO:", 0, {0, 0, 0, 255});
    if(sSub) {
        SDL_Texture* t = SDL_CreateTextureFromSurface(renderer, sSub);
        SDL_FRect r = { 50.0f, yStart, (float)sSub->w, (float)sSub->h };
        SDL_RenderTexture(renderer, t, nullptr, &r);
        SDL_DestroyTexture(t);
        SDL_DestroySurface(sSub);
    }

    SDL_Surface* sup = TTF_RenderText_Blended_Wrapped(fuenteTexto, recetaAMostrar.obtenerProcedimiento().c_str(), 0, {60, 60, 60, 255}, w - 100);
    if (sup) {
        SDL_Texture* tProc = SDL_CreateTextureFromSurface(renderer, sup);
        if (tProc) {
            SDL_FRect r = { 50.0f, yStart + 30.0f, (float)sup->w, (float)sup->h };
            if (r.h > (h - yStart - 100)) r.h = h - yStart - 100;
            SDL_RenderTexture(renderer, tProc, nullptr, &r);
            SDL_DestroyTexture(tProc);
        }
        SDL_DestroySurface(sup);
    }
}



