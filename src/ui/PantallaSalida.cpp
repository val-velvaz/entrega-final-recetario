#include "ui/PantallaSalida.hpp"
#include "Game.hpp"
#include "persistence/RutasAssets.hpp"
#include "utils/RenderizadorTextos.hpp"
#include <iostream>
#include <vector>
#include <cmath>

// --- SISTEMA DE PARTÍCULAS (ESTÁTICO PARA ESTA UNIDAD) ---
struct Gota {
    float x, y;
    float w, h;
    float velocidad;
    SDL_Color color;
};

static std::vector<Gota> lluvia;

// Colores pastel para la lluvia
static const std::vector<SDL_Color> paletaPastel = {
    {255, 182, 193, 200}, // Rosa
    {173, 216, 230, 200}, // Azul Claro
    {255, 253, 208, 200}, // Crema
    {152, 251, 152, 200}, // Verde Palido
    {221, 160, 221, 200}  // Ciruela
};

PantallaSalida::PantallaSalida() 
    : fuente(nullptr), textoBase(nullptr), 
      tiempoAcumulado(0.0f), puntos(0), ciclosCompletados(0), ultimoTick(0) {
    
    // Reiniciar lluvia cada vez que entramos
    lluvia.clear();
    for (int i = 0; i < 100; ++i) { // 100 gotas iniciales
        Gota g;
        g.x = (float)(rand() % 1024);
        g.y = (float)(rand() % 768) - 768; // Empezar arriba
        g.w = (float)(rand() % 5 + 2);     // Ancho 2-7px
        g.h = (float)(rand() % 40 + 20);   // Alto 20-60px
        g.velocidad = (float)(rand() % 10 + 5); // Velocidad
        g.color = paletaPastel[rand() % paletaPastel.size()];
        lluvia.push_back(g);
    }
}

PantallaSalida::~PantallaSalida() {
    cleanup();
}

void PantallaSalida::init(Game& game) {
    SDL_Renderer* renderer = game.getRenderer();
    int w, h;
    SDL_GetRenderOutputSize(renderer, &w, &h);

    fuente = TTF_OpenFont(RutasAssets::obtenerRutaFuenteNegrita().c_str(), 48);
    
    textoBase = RenderizadorTextos::renderizarTexto(renderer, fuente, "CERRANDO RECETARIO", {50, 50, 60, 255});
    
    if (textoBase) {
        rectTexto.x = (w - textoBase->w) / 2.0f;
        rectTexto.y = (h - textoBase->h) / 2.0f;
        rectTexto.w = (float)textoBase->w;
        rectTexto.h = (float)textoBase->h;
    }

    ultimoTick = SDL_GetTicks();
    tiempoAcumulado = 0;
    puntos = 0;
    ciclosCompletados = 0;
}

void PantallaSalida::cleanup() {
    if (textoBase) { SDL_DestroyTexture(textoBase); textoBase = nullptr; }
    if (fuente) { TTF_CloseFont(fuente); fuente = nullptr; }
}

void PantallaSalida::handleEvents(Game& game) {
    SDL_Event evento;
    while (SDL_PollEvent(&evento)) {
        if (evento.type == SDL_EVENT_QUIT) {
            game.setEstaCorriendo(false);
        }
    }
}

void PantallaSalida::update(Game& game) {
    Uint64 actual = SDL_GetTicks();
    float dt = (actual - ultimoTick) / 1000.0f; // Delta time en segundos
    // Cap de dt para evitar saltos gigantes si se traba
    if (dt > 0.1f) dt = 0.1f; 
    
    ultimoTick = actual;

    tiempoAcumulado += dt;

    // Actualizar lógica de salida
    static float tiempoPuntos = 0.0f;
    tiempoPuntos += dt;

    if (tiempoPuntos >= 0.5f) {
        tiempoPuntos = 0.0f;
        puntos++;
        if (puntos > 3) {
            puntos = 0;
            ciclosCompletados++;
        }
    }

    if (ciclosCompletados >= 2) {
        game.setEstaCorriendo(false);
    }

    // --- ANIMAR LLUVIA ---
    int w, h;
    SDL_GetRenderOutputSize(game.getRenderer(), &w, &h);

    for (auto& g : lluvia) {
        g.y += g.velocidad * (dt * 60.0f); // Ajuste de velocidad por frame
        
        // Si sale de la pantalla, reciclar arriba
        if (g.y > h) {
            g.y = -(float)(rand() % 200); // Un poco mas arriba
            g.x = (float)(rand() % w);
        }
    }
}

void PantallaSalida::render(Game& game) {
    SDL_Renderer* renderer = game.getRenderer();
    int w, h;
    SDL_GetRenderOutputSize(renderer, &w, &h);

    // Fondo Gris Oscuro Suave (Para que resalten los colores pastel)
    SDL_SetRenderDrawColor(renderer, 40, 40, 45, 255); 
    SDL_FRect bg = {0, 0, (float)w, (float)h};
    SDL_RenderFillRect(renderer, &bg);

    // DIBUJAR LLUVIA
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    for (const auto& g : lluvia) {
        SDL_SetRenderDrawColor(renderer, g.color.r, g.color.g, g.color.b, g.color.a);
        SDL_FRect rGota = {g.x, g.y, g.w, g.h};
        SDL_RenderFillRect(renderer, &rGota);
    }
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);

    // CAJA CENTRAL (Para que se lea el texto)
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 220); // Blanco semitransparente
    SDL_FRect box = { rectTexto.x - 40, rectTexto.y - 20, rectTexto.w + 120, rectTexto.h + 40 };
    SDL_RenderFillRect(renderer, &box);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);

    // TEXTO
    if (textoBase) {
        SDL_FRect r = rectTexto;
        r.x -= 20.0f; 
        SDL_RenderTexture(renderer, textoBase, nullptr, &r);
    }

    // PUNTOS ANIMADOS
    std::string strPuntos = "";
    for(int i=0; i<puntos; ++i) strPuntos += ".";
    
    if (!strPuntos.empty()) {
        // Color gris oscuro para el texto sobre blanco
        SDL_Texture* tPuntos = RenderizadorTextos::renderizarTexto(renderer, fuente, strPuntos, {50, 50, 60, 255});
        if (tPuntos) {
            SDL_FRect rP = {
                rectTexto.x + rectTexto.w - 15.0f,
                rectTexto.y,
                (float)tPuntos->w,
                (float)tPuntos->h
            };
            SDL_RenderTexture(renderer, tPuntos, nullptr, &rP);
            SDL_DestroyTexture(tPuntos);
        }
    }
}
