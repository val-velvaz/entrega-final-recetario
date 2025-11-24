#include "entities/Receta.hpp"
#include "entities/Ingrediente.hpp"

#include <iostream>
#include <istream>
#include <sstream>
#include <limits>

std::istream& operator>>(std::istream& entrada, Receta& receta) {
    receta.ingredientesListaOrdenada.limpiarLista();

    std::getline(entrada, receta.nombrePlatillo);
    entrada >> receta.autorReceta;

    int categoriaInt;
    entrada >> categoriaInt;
    // CRÍTICO: Consumir el resto de la línea después de la lectura numérica
    if (!entrada.fail()) {
        entrada.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
    receta.categoriaReceta = static_cast<Categoria>(categoriaInt);

    entrada >> receta.tiempoPreparacionMinutos;
    // CRÍTICO: Consumir el resto de la línea después de la lectura numérica
    if (!entrada.fail()) {
        entrada.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
    std::getline(entrada, receta.procedimientoPasos);

    std::getline(entrada, receta.rutaImagenPlatillo);

    int cantidadIngredientes = 0;
    // Debemos verificar si la lectura fue exitosa antes de usar el valor
    if (!(entrada >> cantidadIngredientes)) {
        return entrada;
    }
    // CRÍTICO: Consumir el resto de la línea después de la lectura numérica
    if (!entrada.fail()) {
        entrada.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    for (int i = 0; i < cantidadIngredientes; ++i) {
        Ingrediente ing;
        entrada >> ing;
        if (!entrada.fail()) {
            receta.ingredientesListaOrdenada.agregarAlFinal(ing);
        } else {
            entrada.clear();
            break;
        }
    }
    return entrada;
}
