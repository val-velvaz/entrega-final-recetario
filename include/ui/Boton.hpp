#pragma once
#include <SDL3/SDL.h>
#include <string>

class Boton {
private:
    SDL_FRect rectOriginal;
    SDL_FRect rectActual;
    SDL_Texture* textura;
    bool activo;
    bool hover;

public:
    Boton(SDL_Renderer* renderer, const std::string& rutaImagen, float x, float y, float w, float h);
    ~Boton();

    // NUEVO: Permite cambiar la textura del botón dinámicamente
    void cambiarImagen(SDL_Renderer* renderer, const std::string& nuevaRuta);

    void actualizar();
    void render(SDL_Renderer* renderer);
    bool estaPresionado(float mouseX, float mouseY) const;
    
    void setActivo(bool a) { activo = a; }
    bool esActivo() const { return activo; }
};
