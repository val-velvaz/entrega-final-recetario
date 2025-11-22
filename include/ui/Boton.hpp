#pragma once
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <string>
#include <iostream>

class Boton {
private:
    SDL_Texture* textura;
    SDL_FRect rectangulo;

public:
    // Constructor: Carga la imagen y define posición
    Boton(SDL_Renderer* renderer, const std::string& rutaImagen, float x, float y, float w, float h);
    
    // Destructor: Libera la memoria de la textura automáticamente
    ~Boton();

    // Dibuja el botón en pantalla
    void render(SDL_Renderer* renderer);

    // Retorna true si las coordenadas (x, y) están dentro del botón
    bool estaPresionado(float mouseX, float mouseY) const;

    // Getters por si necesitas moverlo o cambiar tamaño
    SDL_FRect getRect() const { return rectangulo; }
    void setPosicion(float x, float y) { rectangulo.x = x; rectangulo.y = y; }
};