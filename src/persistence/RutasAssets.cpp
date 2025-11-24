// ============================================================================
// ARCHIVO: src/persistence/RutasAssets.cpp
// Implementación del gestor de rutas
// ============================================================================
#include "persistence/RutasAssets.hpp"
// #include "utils/ConvertidoresFormatos.hpp" // Ya no lo necesitamos aquí gracias al switch
#include <iostream> 

// ============================================================================
// INICIALIZACIÓN DE CONSTANTES ESTÁTICAS
// ============================================================================

const std::string RutasAssets::RUTA_RELATIVA_ASSETS = "assets";
const std::string RutasAssets::RUTA_RELATIVA_FONTS = "assets/fonts";
const std::string RutasAssets::RUTA_RELATIVA_ICONS = "assets/images/icons";
const std::string RutasAssets::RUTA_RELATIVA_PLATILLOS = "assets/images/platillos";
const std::string RutasAssets::RUTA_RELATIVA_DATA = "assets/data";

// ============================================================================
// RUTAS A FUENTES (FONTS)
// ============================================================================

std::string RutasAssets::obtenerRutaFuenteRegular() {
    return RUTA_RELATIVA_FONTS + "/font.ttf";
}

std::string RutasAssets::obtenerRutaFuenteNegrita() {
    return RUTA_RELATIVA_FONTS + "/font.ttf";
}

// ========================================================================
// RUTAS A ICONOS Y BOTONES
// ========================================================================

std::string RutasAssets::obtenerRutaIconoBotonAgregar() {
    return RUTA_RELATIVA_ICONS + "/boton_agregar.png";
}

std::string RutasAssets::obtenerRutaIconoBotonEditar() {
    return RUTA_RELATIVA_ICONS + "/boton_editar.png";
}

std::string RutasAssets::obtenerRutaIconoBotonEliminar() {
    return RUTA_RELATIVA_ICONS + "/boton_eliminar.png";
}

std::string RutasAssets::obtenerRutaIconoBotonVolver() {
    return RUTA_RELATIVA_ICONS + "/boton_volver.png";
}

std::string RutasAssets::obtenerRutaIconoBotonGuardar() {
    return RUTA_RELATIVA_ICONS + "/boton_guardar.png";
}

// CORRECCIÓN: Mapeo manual para evitar problemas con caracteres especiales (ñ, acentos)
std::string RutasAssets::obtenerRutaCarpetaImagenesPorCategoria(const Categoria& categoria) {
    switch (categoria) {
        case Categoria::DESAYUNO:
            return RUTA_RELATIVA_PLATILLOS + "/desayuno";
        case Categoria::COMIDA:
            return RUTA_RELATIVA_PLATILLOS + "/comida";
        case Categoria::CENA:
            return RUTA_RELATIVA_PLATILLOS + "/cena";
        case Categoria::NAVIDENO:
            // IMPORTANTE: La carpeta en disco debe llamarse "navideno" (sin ñ)
            return RUTA_RELATIVA_PLATILLOS + "/navideno"; 
        case Categoria::SIN_CATEGORIZAR:
        default:
            return RUTA_RELATIVA_PLATILLOS + "/otros";
    }
}

std::string RutasAssets::obtenerRutaImagenPlatillo(
    const Categoria& categoria,
    const std::string& nombrePlatillo)
{
    // Por ahora devolvemos una imagen por defecto, luego podrás buscar por nombre real
    std::string nombreArchivo = "default.png"; 
    return obtenerRutaCarpetaImagenesPorCategoria(categoria) + "/" + nombreArchivo;
}

std::string RutasAssets::obtenerRutaArchivoRecetario() {
    return RUTA_RELATIVA_DATA + "/recetario.dat";
}