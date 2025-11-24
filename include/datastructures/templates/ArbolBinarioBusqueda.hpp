#pragma once
#include <iostream>
#include <functional>

template <typename T>
struct NodoArbol {
    T dato;
    NodoArbol* izquierda;
    NodoArbol* derecha;

    NodoArbol(const T& valor) : dato(valor), izquierda(nullptr), derecha(nullptr) {}
};

template <typename T>
class ArbolBinarioBusqueda {
private:
    NodoArbol<T>* raiz;

    void insertarRecursivo(NodoArbol<T>*& nodo, const T& valor, std::function<int(const T&, const T&)> comparador) {
        if (nodo == nullptr) {
            nodo = new NodoArbol<T>(valor);
        } else {
            int comparacion = comparador(valor, nodo->dato);
            if (comparacion < 0) {
                insertarRecursivo(nodo->izquierda, valor, comparador);
            } else if (comparacion > 0) {
                insertarRecursivo(nodo->derecha, valor, comparador);
            }
        }
    }

    T* buscarRecursivo(NodoArbol<T>* nodo, const T& valorBusqueda, std::function<int(const T&, const T&)> comparador) const {
        if (nodo == nullptr) {
            return nullptr;
        }
        int comparacion = comparador(valorBusqueda, nodo->dato);
        if (comparacion == 0) {
            return &(nodo->dato);
        } else if (comparacion < 0) {
            return buscarRecursivo(nodo->izquierda, valorBusqueda, comparador);
        } else {
            return buscarRecursivo(nodo->derecha, valorBusqueda, comparador);
        }
    }

    void destruirRecursivo(NodoArbol<T>* nodo) {
        if (nodo) {
            destruirRecursivo(nodo->izquierda);
            destruirRecursivo(nodo->derecha);
            delete nodo;
        }
    }

public:
    ArbolBinarioBusqueda() : raiz(nullptr) {}

    ~ArbolBinarioBusqueda() {
        destruirRecursivo(raiz);
    }

    void insertar(const T& valor, std::function<int(const T&, const T&)> comparador) {
        insertarRecursivo(raiz, valor, comparador);
    }

    T* buscar(const T& valorBusqueda, std::function<int(const T&, const T&)> comparador) const {
        return buscarRecursivo(raiz, valorBusqueda, comparador);
    }

    bool estaVacio() const {
        return raiz == nullptr;
    }
    
    void limpiar() {
        destruirRecursivo(raiz);
        raiz = nullptr;
    }

};
