#include <SDL3/SDL.h>
#include "Game.hpp"
#include <iostream>

// 🔥 CORRECCIÓN: main estándar para SDL3
int main(int argc, char* argv[]) {
    // Silenciar parámetros no usados
    (void)argc;
    (void)argv;
    
    try {
        Game recetarioGame;
        if (recetarioGame.estaCorriendoApp()) {
            recetarioGame.run();
        }
    } catch (const std::exception& e) {
        std::cerr << "Error catastrofico: " << e.what() << std::endl;
        return -1;
    } catch (...) {
        std::cerr << "Error desconocido." << std::endl;
        return -1;
    }
    return 0;
}