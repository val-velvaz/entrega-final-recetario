#pragma once

#include "NodoCircularDoble.hpp"
#include <stdexcept>

template <typename TipoDato>
class ListaCircularDoble {
private:
    NodoCircularDoble<TipoDato>* cabeza;
    int cantidadElementos;

public:
    ListaCircularDoble() : cabeza(nullptr), cantidadElementos(0) {}
    
    ~ListaCircularDoble() {
        limpiar();
    }
    
    void agregarAlFinal(const TipoDato& dato) {
        NodoCircularDoble<TipoDato>* nuevo = new NodoCircularDoble<TipoDato>(dato);
        
        if (!cabeza) {
            cabeza = nuevo;
            cabeza->punteroSiguienteNodo = cabeza;
            cabeza->punteroAnteriorNodo = cabeza;
        } else {
            NodoCircularDoble<TipoDato>* ultimo = cabeza->punteroAnteriorNodo;
            
            ultimo->punteroSiguienteNodo = nuevo;
            nuevo->punteroAnteriorNodo = ultimo;
            nuevo->punteroSiguienteNodo = cabeza;
            cabeza->punteroAnteriorNodo = nuevo;
        }
        cantidadElementos++;
    }
    
    void eliminarDelFrente() {
        if (estaVacia()) throw std::underflow_error("Lista vacía");
        
        if (cantidadElementos == 1) {
            delete cabeza;
            cabeza = nullptr;
        } else {
            NodoCircularDoble<TipoDato>* viejaCabeza = cabeza;
            NodoCircularDoble<TipoDato>* ultimo = cabeza->punteroAnteriorNodo;
            
            cabeza = cabeza->punteroSiguienteNodo;
            cabeza->punteroAnteriorNodo = ultimo;
            ultimo->punteroSiguienteNodo = cabeza;
            
            delete viejaCabeza;
        }
        cantidadElementos--;
    }
    
    bool estaVacia() const { return cantidadElementos == 0; }
    int obtenerTamano() const { return cantidadElementos; }
    
    void limpiar() {
        while (!estaVacia()) {
            eliminarDelFrente();
        }
    }
};