#include "ui/CajaDeTexto.hpp"

CajaDeTexto::CajaDeTexto(float x, float y, float w, float h, TTF_Font* f)
    : rect{x, y, w, h}, texto(""), tieneFoco(false), fuente(f), texturaTexto(nullptr) {}

CajaDeTexto::~CajaDeTexto() {
    if (texturaTexto) SDL_DestroyTexture(texturaTexto);
}

void CajaDeTexto::regenerarTextura(SDL_Renderer* renderer) {
    if (texturaTexto) {
        SDL_DestroyTexture(texturaTexto);
        texturaTexto = nullptr;
    }
    
    if (texto.empty()) return;

    // SDL3: Recordar el argumento '0' para longitud automática
    SDL_Surface* sup = TTF_RenderText_Blended(fuente, texto.c_str(), 0, colorTexto);
    if (sup) {
        texturaTexto = SDL_CreateTextureFromSurface(renderer, sup);
        SDL_DestroySurface(sup);
    }
}

void CajaDeTexto::manejarEventos(const SDL_Event& evento) {
    // 1. Detectar clic para dar foco
    if (evento.type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
        float mx = evento.button.x;
        float my = evento.button.y;
        bool clicDentro = (mx >= rect.x && mx <= rect.x + rect.w && my >= rect.y && my <= rect.y + rect.h);
        
        if (clicDentro && !tieneFoco) {
            tieneFoco = true;
            SDL_StartTextInput(NULL); // Iniciar captura de texto SDL3
        } else if (!clicDentro && tieneFoco) {
            tieneFoco = false;
            SDL_StopTextInput(NULL);
        }
    }
    
    // 2. Capturar texto escrito (SDL3)
    if (tieneFoco) {
        if (evento.type == SDL_EVENT_TEXT_INPUT) {
            texto += evento.text.text;
            // Necesitamos el renderer para regenerar, pero handleEvents no lo tiene.
            // Truco: Marcar flag "sucio" o pasar renderer a handleEvents. 
            // Por simplicidad académica, asumimos que se regenerará en render().
        }
        else if (evento.type == SDL_EVENT_KEY_DOWN) {
            if (evento.key.key == SDLK_BACKSPACE && !texto.empty()) {
                // Borrar último caracter (cuidado con UTF-8, esto es simple para ASCII)
                texto.pop_back(); 
            }
        }
    }
}

void CajaDeTexto::renderizar(SDL_Renderer* renderer) {
    // Regenerar textura si es necesario (aquí un poco ineficiente, mejor optimizar con flag)
    regenerarTextura(renderer);

    // Dibujar fondo
    SDL_SetRenderDrawColor(renderer, colorFondo.r, colorFondo.g, colorFondo.b, colorFondo.a);
    SDL_RenderFillRect(renderer, &rect);

    // Dibujar borde (Verde si tiene foco, Negro si no)
    if (tieneFoco) SDL_SetRenderDrawColor(renderer, 0, 200, 0, 255);
    else SDL_SetRenderDrawColor(renderer, colorBorde.r, colorBorde.g, colorBorde.b, colorBorde.a);
    SDL_RenderRect(renderer, &rect);

    // Dibujar texto
    if (texturaTexto) {
        float w, h;
        SDL_GetTextureSize(texturaTexto, &w, &h);
        // Centrar verticalmente
        SDL_FRect dst = { rect.x + 5, rect.y + (rect.h - h)/2, w, h };
        SDL_RenderTexture(renderer, texturaTexto, nullptr, &dst);
    }
}