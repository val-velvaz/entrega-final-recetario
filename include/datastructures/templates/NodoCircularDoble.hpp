#pragma once

#include <cstddef>
#include <utility>

template <typename TipoDato>
class NodoCircularDoble {
public:
    TipoDato datosAlmacenados;
    NodoCircularDoble<TipoDato>* punteroAnteriorNodo;
    NodoCircularDoble<TipoDato>* punteroSiguienteNodo;

    NodoCircularDoble(const TipoDato& datosIngresados)
        : datosAlmacenados(datosIngresados),
          punteroAnteriorNodo(nullptr),
          punteroSiguienteNodo(nullptr) {
    }

    NodoCircularDoble(TipoDato&& datosIngresados)
        : datosAlmacenados(std::move(datosIngresados)),
          punteroAnteriorNodo(nullptr),
          punteroSiguienteNodo(nullptr) {
    }

    ~NodoCircularDoble() = default;

    bool tieneNodoSiguiente() const {
        return punteroSiguienteNodo != nullptr;
    }
    
    bool tieneNodoAnterior() const {
        return punteroAnteriorNodo != nullptr;
    }

    void vincularAnterior(NodoCircularDoble<TipoDato>* nodoAnterior) {
        punteroAnteriorNodo = nodoAnterior;
    }

    void vincularSiguiente(NodoCircularDoble<TipoDato>* nodoSiguiente) {
        punteroSiguienteNodo = nodoSiguiente;
    }

    void vincularBidireccional(NodoCircularDoble<TipoDato>* nodoAnterior,
                               NodoCircularDoble<TipoDato>* nodoSiguiente) {
        punteroAnteriorNodo = nodoAnterior;
        punteroSiguienteNodo = nodoSiguiente;
    }

    NodoCircularDoble<TipoDato>* obtenerNodoSiguiente() const {
        return punteroSiguienteNodo;
    }

    NodoCircularDoble<TipoDato>* obtenerNodoAnterior() const {
        return punteroAnteriorNodo;
    }

    TipoDato& obtenerDatos() {
        return datosAlmacenados;
    }

    const TipoDato& obtenerDatosConstantes() const {
        return datosAlmacenados;
    }

    bool estaDesvinculado() const {
        return punteroAnteriorNodo == nullptr && punteroSiguienteNodo == nullptr;
    }

    bool esCircular() const {
        return (punteroAnteriorNodo == this && punteroSiguienteNodo == this);
    }
};
