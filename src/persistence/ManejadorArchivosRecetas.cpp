#include "persistence/ManejadorArchivosRecetas.hpp"
#include <fstream>
#include <iostream>
#include <cstdio>

// Implementación de métodos estáticos

bool ManejadorArchivosRecetas::verificarIntegridad(const std::string& rutaArchivo) {
    std::ifstream archivo(rutaArchivo);
    return archivo.good();
}

void ManejadorArchivosRecetas::crearRespaldo(const std::string& rutaArchivo) {
    std::ifstream src(rutaArchivo, std::ios::binary);
    std::ofstream dst(rutaArchivo + ".bak", std::ios::binary);
    dst << src.rdbuf();
}

InfoArchivo ManejadorArchivosRecetas::obtenerInformacionArchivo(const std::string& rutaArchivo) {
    InfoArchivo info;
    info.ruta = rutaArchivo;
    std::ifstream f(rutaArchivo, std::ios::binary | std::ios::ate);
    if (f.good()) {
        info.existe = true;
        info.tamanoByte = f.tellg();
    } else {
        info.existe = false;
        info.tamanoByte = 0;
    }
    return info;
}

bool ManejadorArchivosRecetas::crearArchivoSiNoExiste(const std::string& rutaArchivo) {
    std::ifstream f(rutaArchivo);
    if (!f.good()) {
        std::ofstream nuevo(rutaArchivo);
        if (nuevo.good()) {
            nuevo.close();
            return true;
        }
        return false;
    }
    return true;
}

bool ManejadorArchivosRecetas::eliminarArchivo(const std::string& rutaArchivo) {
    return std::remove(rutaArchivo.c_str()) == 0;
}

bool ManejadorArchivosRecetas::renombrarArchivo(const std::string& rutaActual, const std::string& rutaNueva) {
    return std::rename(rutaActual.c_str(), rutaNueva.c_str()) == 0;
}

bool ManejadorArchivosRecetas::crearRespaldoAutomatico(const std::string& rutaArchivo) {
    crearRespaldo(rutaArchivo);
    return true;
}

bool ManejadorArchivosRecetas::restaurarDesdeRespaldo(const std::string& rutaRespaldo, const std::string& rutaDestino) {
    std::ifstream src(rutaRespaldo, std::ios::binary);
    std::ofstream dst(rutaDestino, std::ios::binary);
    if (src.good() && dst.good()) {
        dst << src.rdbuf();
        return true;
    }
    return false;
}

void ManejadorArchivosRecetas::limpiarArchivosAntiguos(const std::string& ruta, int diasAntiguedad) {
    (void)ruta;
    (void)diasAntiguedad;
}
