#include "utils/ConvertidoresFormatos.hpp"
#include <iomanip>
#include <sstream>
#include <algorithm>

std::string ConvertidoresFormatos::enteroAstring(int numero) {
    return std::to_string(numero);
}

std::string ConvertidoresFormatos::floatAstring(float numero, int decimales) {
    std::stringstream stream;
    stream << std::fixed << std::setprecision(decimales) << numero;
    return stream.str();
}

std::string ConvertidoresFormatos::boolAtexto(bool valor) {
    return valor ? "Si" : "No";
}

int ConvertidoresFormatos::stringAentero(const std::string& texto) {
    try {
        return std::stoi(texto);
    } catch (...) {
        return 0;
    }
}

float ConvertidoresFormatos::stringAfloat(const std::string& texto) {
    try {
        return std::stof(texto);
    } catch (...) {
        return 0.0f;
    }
}

std::string ConvertidoresFormatos::limpiarEspacios(const std::string& texto) {
    std::string resultado = texto;
    // Trim izquierda
    resultado.erase(resultado.begin(), std::find_if(resultado.begin(), resultado.end(), [](unsigned char ch) {
        return !std::isspace(ch);
    }));
    // Trim derecha
    resultado.erase(std::find_if(resultado.rbegin(), resultado.rend(), [](unsigned char ch) {
        return !std::isspace(ch);
    }).base(), resultado.end());
    return resultado;
}

std::string ConvertidoresFormatos::convertirAMayusculas(const std::string& texto) {
    std::string resultado = texto;
    std::transform(resultado.begin(), resultado.end(), resultado.begin(), ::toupper);
    return resultado;
}

std::string ConvertidoresFormatos::padearIzquierda(const std::string& texto, int ancho, char relleno) {
    if (texto.length() >= (size_t)ancho) {
        return texto;
    }
    return std::string(ancho - texto.length(), relleno) + texto;
}

std::string ConvertidoresFormatos::padearDerecha(const std::string& texto, int ancho, char relleno) {
    if (texto.length() >= (size_t)ancho) {
        return texto;
    }
    return texto + std::string(ancho - texto.length(), relleno);
}
