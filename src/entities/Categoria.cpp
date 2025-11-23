#include "entities/Categoria.hpp"

std::string categoriaATexto(Categoria categoria) {
    switch (categoria) {
        case Categoria::DESAYUNO: return "Desayuno";
        case Categoria::COMIDA:   return "Comida";
        case Categoria::CENA:     return "Cena";
        case Categoria::NAVIDENO: return "Navideno";
        default:                  return "Sin Categorizar";
    }
}
