#include "ui/CajaDeTexto.hpp"
#include "utils/RenderizadorTextos.hpp"

CajaDeTexto::CajaDeTexto(SDL_Window* win, float x, float y, float w, float h, TTF_Font* f)
    : ventana(win), rect{x, y, w, h}, texto(""), placeholder(""), tieneFoco(false), fuente(f), texturaTexto(nullptr) {}

CajaDeTexto::~CajaDeTexto() {
    if (texturaTexto) {
        SDL_DestroyTexture(texturaTexto);
    }
}

void CajaDeTexto::establecerTexto(const std::string& t) {
    texto = t;
    if (texturaTexto) {
        SDL_DestroyTexture(texturaTexto);
        texturaTexto = nullptr;
    }
}

void CajaDeTexto::manejarEvento(const SDL_Event& evento) {
    if (evento.type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
        float mx = evento.button.x;
        float my = evento.button.y;
        bool clicDentro = (mx >= rect.x && mx <= rect.x + rect.w &&
                           my >= rect.y && my <= rect.y + rect.h);
        
        if (clicDentro && !tieneFoco) {
            tieneFoco = true;
            SDL_StartTextInput(ventana); 
        } else if (!clicDentro && tieneFoco) {
            tieneFoco = false;
            SDL_StopTextInput(ventana);
        }
    }
    else if (tieneFoco && evento.type == SDL_EVENT_TEXT_INPUT) {
        texto += evento.text.text;
        if (texturaTexto) { SDL_DestroyTexture(texturaTexto); texturaTexto = nullptr; }
    }
    else if (tieneFoco && evento.type == SDL_EVENT_KEY_DOWN) {
        if (evento.key.key == SDLK_BACKSPACE && !texto.empty()) {
            texto.pop_back();
            if (texturaTexto) { SDL_DestroyTexture(texturaTexto); texturaTexto = nullptr; }
        }
    }
}

void CajaDeTexto::regenerarTextura(SDL_Renderer* renderer) {
    if (texturaTexto) SDL_DestroyTexture(texturaTexto);
    texturaTexto = nullptr;

    if (texto.empty() && placeholder.empty()) return;

    std::string textoAmostrar = texto.empty() ? placeholder : texto;
    // Color gris suave para placeholder, negro solido para texto real
    SDL_Color color = texto.empty() ? SDL_Color{150, 150, 150, 255} : SDL_Color{50, 50, 50, 255};

    texturaTexto = RenderizadorTextos::renderizarTexto(renderer, fuente, textoAmostrar, color);
}

// --- AQUÍ ESTÁ EL NUEVO DISEÑO ---
void CajaDeTexto::render(SDL_Renderer* renderer) {
    // Habilitar blending para transparencias (sombras)
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    // 1. SOMBRA (Depth Effect)
    // Dibujamos un rectangulo gris semitransparente desplazado ligeramente
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 30); // Negro al 12% opacidad
    SDL_FRect rSombra = {rect.x + 4, rect.y + 4, rect.w, rect.h};
    SDL_RenderFillRect(renderer, &rSombra);

    // 2. FONDO (Blanco Puro)
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderFillRect(renderer, &rect);

    // 3. INDICADOR DE ESTADO (Borde Inferior)
    if (tieneFoco) {
        // ACTIVO: Barra inferior gruesa de color "Azul Cornflower" (Pastel intenso)
        SDL_SetRenderDrawColor(renderer, 100, 149, 237, 255); 
        SDL_FRect rLinea = {rect.x, rect.y + rect.h - 3, rect.w, 3}; // 3px de alto
        SDL_RenderFillRect(renderer, &rLinea);
    } else {
        // INACTIVO: Borde sutil gris alrededor (Estilo tarjeta)
        SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
        SDL_RenderRect(renderer, &rect);
    }
    
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);

    // 4. TEXTO
    if (!texturaTexto) {
        regenerarTextura(renderer);
    }

    if (texturaTexto) {
        float tw = (float)texturaTexto->w;
        float th = (float)texturaTexto->h;
        
        // Padding (margen interno) para que el texto no pegue al borde
        float paddingX = 10.0f;
        // Centrado vertical
        float posY = rect.y + (rect.h - th) / 2.0f;
        
        // Si es multilínea o muy alto (como el campo de procedimiento), alinear arriba
        if (rect.h > 50) { 
            posY = rect.y + 10.0f; 
        }

        // Clipping: Evitar que el texto se salga de la caja si es muy largo
        SDL_SetRenderClipRect(renderer, (const SDL_Rect*)&rect);
        
        SDL_FRect dst = { rect.x + paddingX, posY, tw, th };
        SDL_RenderTexture(renderer, texturaTexto, nullptr, &dst);
        
        SDL_SetRenderClipRect(renderer, nullptr); // Desactivar clip
    }
}
