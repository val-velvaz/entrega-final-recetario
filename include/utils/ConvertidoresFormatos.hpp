#pragma once
#include <string>

class ConvertidoresFormatos {
public:
    static std::string enteroAstring(int numero);
    static std::string floatAstring(float numero, int decimales);
    static std::string boolAtexto(bool valor);
    
    static int stringAentero(const std::string& texto);
    static float stringAfloat(const std::string& texto);
    
    static std::string limpiarEspacios(const std::string& texto);
    static std::string convertirAMayusculas(const std::string& texto);
    
    static std::string padearIzquierda(const std::string& texto, int ancho, char relleno);
    static std::string padearDerecha(const std::string& texto, int ancho, char relleno);
};
