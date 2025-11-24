#pragma once
#include <string>

enum class Categoria {
    DESAYUNO = 0,
    COMIDA,
    CENA,
    NAVIDENO,
    SIN_CATEGORIZAR
};

std::string categoriaATexto(Categoria categoria);

