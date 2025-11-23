#pragma once
#include <string>

enum class Categoria {
    DESAYUNO = 0,
    COMIDA,
    CENA,
    NAVIDENO,
    SIN_CATEGORIZAR
};

// Declaración de la función para que sea visible globalmente
std::string categoriaATexto(Categoria categoria);
