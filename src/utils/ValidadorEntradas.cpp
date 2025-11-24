#include "utils/ValidadorEntradas.hpp"

// Inicialización de Regex Estáticos
const std::string ValidadorEntradas::REGEX_EMAIL = R"(^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,}$)";
const std::string ValidadorEntradas::REGEX_NOMBRE = R"(^[a-zA-ZáéíóúÁÉÍÓÚñÑ\s]+$)";
const std::string ValidadorEntradas::REGEX_NUMERO_ENTERO = R"(^\d+$)";
const std::string ValidadorEntradas::REGEX_NUMERO_DECIMAL = R"(^\d+(\.\d+)?$)";

bool ValidadorEntradas::verificarQueNoEsteVacio(const std::string& entrada) {
    return !entrada.empty() && std::any_of(entrada.begin(), entrada.end(), [](unsigned char c){ return !std::isspace(c); });
}

bool ValidadorEntradas::verificarSiEsNombreValido(const std::string& nombre) {
    if (!verificarQueNoEsteVacio(nombre)) return false;
    return std::regex_match(nombre, std::regex(REGEX_NOMBRE));
}

bool ValidadorEntradas::verificarSiEsNumeroEnteroPositivo(const std::string& entrada) {
    if (entrada.empty()) return false;
    return std::regex_match(entrada, std::regex(REGEX_NUMERO_ENTERO));
}

bool ValidadorEntradas::verificarSiEsNumeroDecimalPositivo(const std::string& entrada) {
    if (entrada.empty()) return false;
    return std::regex_match(entrada, std::regex(REGEX_NUMERO_DECIMAL));
}

bool ValidadorEntradas::verificarSiEstaEnRango(int valor, int minimo, int maximo) {
    return valor >= minimo && valor <= maximo;
}

// CORREGIDO: int -> size_t para coincidir con el header
bool ValidadorEntradas::verificarLongitudString(const std::string& entrada, size_t longitudMinima, size_t longitudMaxima) {
    size_t len = entrada.length();
    return len >= longitudMinima && len <= longitudMaxima;
}

bool ValidadorEntradas::verificarSiEsEmailValido(const std::string& email) {
    return std::regex_match(email, std::regex(REGEX_EMAIL));
}

bool ValidadorEntradas::verificarSiEsRutaArchivoValida(const std::string& ruta) {
    return !ruta.empty(); 
}

std::string ValidadorEntradas::limpiarYnormalizarEntrada(const std::string& entrada) {
    std::string temp = entrada;
    temp.erase(temp.begin(), std::find_if(temp.begin(), temp.end(), [](unsigned char ch) {
        return !std::isspace(ch);
    }));
    temp.erase(std::find_if(temp.rbegin(), temp.rend(), [](unsigned char ch) {
        return !std::isspace(ch);
    }).base(), temp.end());
    return temp;
}

std::string ValidadorEntradas::convertirAminusculas(const std::string& texto) {
    std::string res = texto;
    std::transform(res.begin(), res.end(), res.begin(), ::tolower);
    return res;
}

std::string ValidadorEntradas::convertirAmayusculas(const std::string& texto) {
    std::string res = texto;
    std::transform(res.begin(), res.end(), res.begin(), ::toupper);
    return res;
}

std::string ValidadorEntradas::capitalizarPrimeraLetra(const std::string& texto) {
    if (texto.empty()) return texto;
    std::string res = texto;
    res[0] = std::toupper(res[0]);
    return res;
}

bool ValidadorEntradas::verificarSiEsOpcionMenuValida(const std::string& opcion, const std::string& opcionesValidas) {
    if (opcion.length() != 1) return false;
    return opcionesValidas.find(opcion) != std::string::npos;
}
