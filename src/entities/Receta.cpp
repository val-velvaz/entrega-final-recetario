#include "entities/Receta.hpp"
#include "entities/Ingrediente.hpp"
#include "entities/Nombre.hpp"
#include "entities/Categoria.hpp"
#include <iostream>
#include <sstream>
#include <limits>

// ============================================================================
// CONSTRUCTORES
// ============================================================================

Receta::Receta() 
    : categoriaReceta(Categoria::SIN_CATEGORIZAR), // Corregido: OTROS -> SIN_CATEGORIZAR
      tiempoPreparacionMinutos(0) {                // Reordenado para coincidir con .hpp
}

Receta::Receta(const std::string& nombrePlatillo_, const Nombre& autor, const Categoria& categoria, int tiempoMinutos, const std::string& procedimiento)
    : nombrePlatillo(nombrePlatillo_), 
      autorReceta(autor), 
      categoriaReceta(categoria), 
      tiempoPreparacionMinutos(tiempoMinutos), 
      procedimientoPasos(procedimiento) {
}

Receta::Receta(const Receta& otraReceta)
    : nombrePlatillo(otraReceta.nombrePlatillo),
      autorReceta(otraReceta.autorReceta),
      categoriaReceta(otraReceta.categoriaReceta),
      tiempoPreparacionMinutos(otraReceta.tiempoPreparacionMinutos),
      procedimientoPasos(otraReceta.procedimientoPasos),
      rutaImagenPlatillo(otraReceta.rutaImagenPlatillo) {
    
    // Copia de ingredientes usando asignación de la lista
    this->ingredientesListaOrdenada = otraReceta.ingredientesListaOrdenada;
}

Receta::Receta(Receta&& otraReceta) noexcept = default;

Receta& Receta::operator=(const Receta& otraReceta) = default;

Receta& Receta::operator=(Receta&& otraReceta) noexcept = default;

// ============================================================================
// COMPARADORES
// ============================================================================

bool Receta::operator==(const Receta& otraReceta) const {
    return this->nombrePlatillo == otraReceta.nombrePlatillo; 
}

bool Receta::operator<(const Receta& otraReceta) const {
    return this->nombrePlatillo < otraReceta.nombrePlatillo;
}

int Receta::compararPorNombre(const Receta& rec1, const Receta& rec2) {
    if (rec1.nombrePlatillo < rec2.nombrePlatillo) return -1;
    if (rec1.nombrePlatillo > rec2.nombrePlatillo) return 1;
    return 0;
}

int Receta::compararPorTiempoPreparacion(const Receta& rec1, const Receta& rec2) {
    if (rec1.tiempoPreparacionMinutos < rec2.tiempoPreparacionMinutos) return -1;
    if (rec1.tiempoPreparacionMinutos > rec2.tiempoPreparacionMinutos) return 1;
    return 0;
}

int Receta::compararPorCategoria(const Receta& rec1, const Receta& rec2) {
    if (rec1.categoriaReceta < rec2.categoriaReceta) return -1;
    if (rec1.categoriaReceta > rec2.categoriaReceta) return 1;
    return 0;
}

int Receta::compararPorAutor(const Receta& rec1, const Receta& rec2) {
    (void)rec1; 
    (void)rec2;
    return 0; 
}

// ============================================================================
// GESTIÓN DE INGREDIENTES
// ============================================================================

void Receta::agregarIngredienteOrdenado(const Ingrediente& ingrediente) {
    ingredientesListaOrdenada.agregarAlFinal(ingrediente);
}

void Receta::eliminarIngredientePorNombre(const std::string& nombreIngrediente) {
    (void)nombreIngrediente;
    // Pendiente
}

void Receta::eliminarTodosLosIngredientes() {
    ingredientesListaOrdenada.limpiarLista();
}

bool Receta::verificarSiIngredienteExiste(const std::string& nombreBuscado) const {
    (void)nombreBuscado;
    return false;
}

Ingrediente& Receta::obtenerIngredientePorNombre(const std::string& nombreBuscado) {
    throw std::runtime_error("Ingrediente no encontrado: " + nombreBuscado);
}

void Receta::modificarCantidadIngrediente(const std::string& nombreIngrediente, const std::string& nuevaCantidad) {
    (void)nombreIngrediente;
    (void)nuevaCantidad;
}

// ============================================================================
// STRINGS Y SERIALIZACIÓN
// ============================================================================

std::string Receta::toString() const {
    return nombrePlatillo;
}

std::string Receta::obtenerResumenCorto() const {
    return nombrePlatillo + " (" + std::to_string(tiempoPreparacionMinutos) + " min)";
}

std::string Receta::obtenerDetalleCompleto() const {
    std::stringstream ss;
    ss << "Platillo: " << nombrePlatillo << "\n"
       << "Tiempo: " << tiempoPreparacionMinutos << " mins\n"
       << "Procedimiento: " << procedimientoPasos;
    return ss.str();
}

std::ostream& operator<<(std::ostream& salida, const Receta& receta) {
    salida << receta.nombrePlatillo << "\n";
    salida << receta.autorReceta << "\n"; 
    salida << static_cast<int>(receta.categoriaReceta) << "\n";
    salida << receta.tiempoPreparacionMinutos << "\n";
    salida << receta.procedimientoPasos << "\n";
    salida << receta.rutaImagenPlatillo << "\n";
    salida << receta.ingredientesListaOrdenada.obtenerCantidadElementos() << "\n";
    return salida;
}

std::istream& operator>>(std::istream& entrada, Receta& receta) {
    receta.ingredientesListaOrdenada.limpiarLista();

    std::getline(entrada, receta.nombrePlatillo);
    if (receta.nombrePlatillo.empty() && !entrada.eof()) { 
        std::getline(entrada, receta.nombrePlatillo); 
    }

    entrada >> receta.autorReceta;

    int categoriaInt;
    entrada >> categoriaInt;
    if (!entrada.fail()) {
        entrada.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
    receta.categoriaReceta = static_cast<Categoria>(categoriaInt);

    entrada >> receta.tiempoPreparacionMinutos;
    if (!entrada.fail()) {
        entrada.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    std::getline(entrada, receta.procedimientoPasos);
    std::getline(entrada, receta.rutaImagenPlatillo);

    int cantidadIngredientes = 0;
    if (!(entrada >> cantidadIngredientes)) {
        return entrada;
    }
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
