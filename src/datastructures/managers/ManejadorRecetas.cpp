// ============================================================================
// ARCHIVO: src/datastructures/managers/ManejadorRecetas.cpp
// Implementación del gestor de recetas (CON PERSISTENCIA)
// ============================================================================
#include "datastructures/managers/ManejadorRecetas.hpp"
#include <stdexcept>
#include <fstream>   // <--- ¡NUEVO! Para leer/escribir archivos
#include <limits>    // <--- ¡NUEVO! Para std::numeric_limits
#include <sstream>

// ========================================================================
// Constructores y Destructores
// ========================================================================

ManejadorRecetas::ManejadorRecetas() {
    // La lista se inicializa vacía por defecto
}

ManejadorRecetas::~ManejadorRecetas() {
    // La lista se destruye automáticamente
}

// ========================================================================
// MÉTODOS AUXILIARES PRIVADOS
// ========================================================================

int ManejadorRecetas::encontrarIndiceRecetaPorNombre(const std::string& nombreBuscado) const {
    for (int i = 0; i < recetasAlmacenadas.obtenerCantidadElementos(); ++i) {
        if (recetasAlmacenadas.obtenerEnPosicion(i).obtenerNombrePlatillo() == nombreBuscado) {
            return i;
        }
    }
    return -1; // No encontrado
}

// ========================================================================
// CRUD - CREATE (Crear)
// ========================================================================

void ManejadorRecetas::agregarRecetaNueva(const Receta& recetaAinsertar) {
    if (verificarSiRecetaExiste(recetaAinsertar.obtenerNombrePlatillo())) {
        throw std::runtime_error("Error: Ya existe una receta con el nombre " + recetaAinsertar.obtenerNombrePlatillo());
    }
    recetasAlmacenadas.agregarAlFinal(recetaAinsertar);
}

Receta& ManejadorRecetas::obtenerRecetaPorNombre(const std::string& nombreBuscado) {
    int indice = encontrarIndiceRecetaPorNombre(nombreBuscado);
    if (indice == -1) {
        throw std::runtime_error("Receta no encontrada: " + nombreBuscado);
    }
    return recetasAlmacenadas.obtenerEnPosicion(indice);
}

const Receta& ManejadorRecetas::obtenerRecetaPorNombreConstante(const std::string& nombreBuscado) const {
    int indice = encontrarIndiceRecetaPorNombre(nombreBuscado);
    if (indice == -1) {
        throw std::runtime_error("Receta no encontrada: " + nombreBuscado);
    }
    return recetasAlmacenadas.obtenerEnPosicion(indice);
}

// ========================================================================
// CRUD - DELETE (Eliminar)
// ========================================================================

void ManejadorRecetas::eliminarRecetaPorNombre(const std::string& nombreAeliminar) {
    int indice = encontrarIndiceRecetaPorNombre(nombreAeliminar);
    if (indice == -1) {
        throw std::runtime_error("No se puede eliminar, receta no encontrada: " + nombreAeliminar);
    }
    recetasAlmacenadas.eliminarEnPosicion(indice);
}

void ManejadorRecetas::eliminarTodasLasRecetas() {
    recetasAlmacenadas.limpiarLista();
}

// ========================================================================
// BÚSQUEDA AVANZADA
// ========================================================================

bool ManejadorRecetas::verificarSiRecetaExiste(const std::string& nombreReceta) const {
    return encontrarIndiceRecetaPorNombre(nombreReceta) != -1;
}

// ========================================================================
// ORDENAMIENTO (Recursivo - QuickSort)
// ========================================================================

// (Aquí va tu implementación existente de QuickSort y particion...)
// ... Asumimos que el código de QuickSort que ya tenías está aquí ...
int particion(ListaDoblementeLigada<Receta>& lista, int izquierda, int derecha, int (*comparador)(const Receta&, const Receta&)) {
    Receta pivote = lista.obtenerEnPosicion(derecha);
    int i = (izquierda - 1);
    for (int j = izquierda; j <= derecha - 1; j++) {
        if (comparador(lista.obtenerEnPosicion(j), pivote) < 0) {
            i++;
            lista.intercambiarDatos(i, j);
        }
    }
    lista.intercambiarDatos(i + 1, derecha);
    return (i + 1);
}

void ManejadorRecetas::quickSortRecursivo(
    ListaDoblementeLigada<Receta>& lista,
    int izquierda,
    int derecha,
    int (*comparador)(const Receta&, const Receta&)) 
{
    if (izquierda < derecha) {
        int pi = particion(lista, izquierda, derecha, comparador);
        quickSortRecursivo(lista, izquierda, pi - 1, comparador);
        quickSortRecursivo(lista, pi + 1, derecha, comparador);
    }
}

void ManejadorRecetas::ordenarRecetasPorNombre() {
    if (obtenerCantidadRecetas() > 1)
        quickSortRecursivo(recetasAlmacenadas, 0, obtenerCantidadRecetas() - 1, Receta::compararPorNombre);
}
void ManejadorRecetas::ordenarRecetasPorTiempoPreparacion() {
    if (obtenerCantidadRecetas() > 1)
        quickSortRecursivo(recetasAlmacenadas, 0, obtenerCantidadRecetas() - 1, Receta::compararPorTiempoPreparacion);
}
void ManejadorRecetas::ordenarRecetasPorCategoria() {
    if (obtenerCantidadRecetas() > 1)
        quickSortRecursivo(recetasAlmacenadas, 0, obtenerCantidadRecetas() - 1, Receta::compararPorCategoria);
}
void ManejadorRecetas::ordenarRecetasPorAutor() {
    if (obtenerCantidadRecetas() > 1)
        quickSortRecursivo(recetasAlmacenadas, 0, obtenerCantidadRecetas() - 1, Receta::compararPorAutor);
}


// ========================================================================
// PERSISTENCIA - Guardar y Cargar (¡IMPLEMENTADO!)
// ========================================================================

void ManejadorRecetas::guardarRecetasAarchivo(const std::string& rutaArchivo) {
    // 1. Abrir archivo de salida (ofstream)
    std::ofstream archivoSalida(rutaArchivo);

    if (!archivoSalida.is_open()) {
        throw std::runtime_error("Error: No se pudo abrir el archivo para guardar: " + rutaArchivo);
    }

    // 2. Escribir la cantidad de recetas
    int cantidad = recetasAlmacenadas.obtenerCantidadElementos();
    archivoSalida << cantidad << std::endl;

    // 3. Recorrer la lista y guardar cada receta
    // (O(n^2) por el acceso, pero O(n) si tuviéramos iterador)
    for (int i = 0; i < cantidad; ++i) {
        // Usamos el operator<< de Receta que ya definimos
        archivoSalida << recetasAlmacenadas.obtenerEnPosicion(i);
    }

    archivoSalida.close();
}

/*
void ManejadorRecetas::cargarRecetasDesdeArchivo(const std::string& rutaArchivo) {
    std::ifstream archivoEntrada(rutaArchivo);
    if (!archivoEntrada.is_open()) {
        return; 
    }

    eliminarTodasLasRecetas();

    // --- ¡CORRECCIÓN VITAL AQUÍ! ---
    int cantidad = 0; // Inicializar en 0
    
    archivoEntrada >> cantidad; // Intentar leer

    // Si la lectura falla (archivo vacío) o es un número inválido, cantidad será 0
    if (archivoEntrada.fail()) {
        cantidad = 0;
    }

    archivoEntrada.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    for (int i = 0; i < cantidad; ++i) {
        Receta recetaTemporal;
        archivoEntrada >> recetaTemporal;
        recetasAlmacenadas.agregarAlFinal(recetaTemporal);
    }

    archivoEntrada.close();
}
// ========================================================================
// ESTADÍSTICAS Y REPORTES
// ========================================================================
*/
void ManejadorRecetas::cargarRecetasDesdeArchivo(const std::string& rutaArchivo) {
    std::ifstream archivoEntrada(rutaArchivo);
    if (!archivoEntrada.is_open()) {
        return; 
    }

    // --- VALIDACIÓN DE SEGURIDAD ---
    // Verificar si el archivo está vacío
    if (archivoEntrada.peek() == std::ifstream::traits_type::eof()) {
        return;
    }

    int cantidad = 0;
    archivoEntrada >> cantidad; 

    if (archivoEntrada.fail()) {
        return;
    }

    // 🛑 FRENO DE EMERGENCIA: 
    // Si el archivo dice que tiene más de 10,000 recetas, es basura o corrupción.
    // Esto evita que tu RAM suba al 95% intentando crear millones de nodos.
    if (cantidad < 0 || cantidad > 10000) {
        std::cerr << "[ALERTA] Archivo corrupto detectado (Cantidad absurda: " << cantidad << "). Se reiniciara la base de datos." << std::endl;
        eliminarTodasLasRecetas(); // Aseguramos limpieza
        return; 
    }

    eliminarTodasLasRecetas();
    archivoEntrada.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    for (int i = 0; i < cantidad; ++i) {
        Receta recetaTemporal;
        try {
            archivoEntrada >> recetaTemporal;
            if (archivoEntrada.fail()) break; // Parar si el archivo se corta inesperadamente
            recetasAlmacenadas.agregarAlFinal(recetaTemporal);
        } catch (...) {
            break; // Salir si hay error al leer una receta individual
        }
    }

    archivoEntrada.close();
}

std::string ManejadorRecetas::obtenerEstadisticasRecetario() const {
    std::stringstream ss;
    ss << "--- Estadisticas del Recetario ---\n";
    ss << "Total de Recetas: " << obtenerCantidadRecetas() << "\n";
    // (Aquí podríamos agregar más estadísticas)
    return ss.str();
}