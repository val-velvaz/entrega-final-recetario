#include <iostream>
#include "datastructures/templates/ListaDoblementeLigada.hpp"

int main() {
    ListaDoblementeLigada<int> lista;
    
    // Test básico
    lista.agregarAlFinal(1);
    lista.agregarAlFinal(2);
    lista.agregarAlFinal(3);
    
    std::cout << "Lista doble: " << lista.obtenerCantidadElementos() << " elementos" << std::endl;
    
    return 0;
}