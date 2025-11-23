#pragma once

#include "ListaSimplementeLigada.hpp"
#include <stdexcept>

template <typename TipoDato>
class ColaDinamica {
private:
    ListaSimplementeLigada<TipoDato> listaContenedora;

public:
    ColaDinamica() = default;
    
    void encolar(const TipoDato& elemento) {
        listaContenedora.agregarAlFinal(elemento);
    }
    
    void desencolar() {
        if (estaVacia()) {
            throw std::underflow_error("Cola vacía");
        }
        listaContenedora.eliminarDelInicio();
    }
    
    TipoDato& frente() {
        if (estaVacia()) {
            throw std::underflow_error("Cola vacía");
        }
        return listaContenedora.obtenerEnPosicion(0);
    }
    
    bool estaVacia() const {
        return listaContenedora.estaVacia();
    }
    
    int obtenerTamano() const {
        return listaContenedora.obtenerCantidadElementos();
    }
    
    void limpiar() {
        listaContenedora.limpiarLista();
    }
};