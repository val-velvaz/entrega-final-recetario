# 📚 Recetario Interactivo: Implementación de Estructuras de Datos Avanzadas (C++ & SDL3)

## 📌 Estado del Proyecto y Resumen Ejecutivo

Este proyecto corresponde a una **aplicación de escritorio para la gestión de recetas**, desarrollada como una implementación práctica y detallada de **estructuras de datos y algoritmos en C++17**. El objetivo principal fue la construcción manual de la lógica de datos y la implementación de algoritmos con complejidad controlada, sin recurrir a contenedores estándar de la STL para la manipulación de los datos principales.

La aplicación cuenta con una **interfaz gráfica funcional** desarrollada con **SDL3**, que permite la interacción completa del usuario.

| Característica | Estado | Observaciones Clave |
|----------------|--------|-------------------|
| Estructuras de Datos | ✅ Implementación Manual | Listas Doblemente Ligadas, Pilas, Colas, entre otras, construidas a partir de nodos dinámicos. |
| Algoritmos Avanzados | ✅ Operativo | Quicksort recursivo, Mergesort, Búsqueda Binaria implementados manualmente. |
| Interfaz Gráfica | ✅ Funcional | Construida con SDL3, SDL3_ttf y SDL3_image. |
| Persistencia (I/O) | ⚠️ Inestable | El módulo de manejo de archivos binarios presenta fallos de integridad que pueden corromper la información al guardar o cargar. |

---

## 🛠️ Tecnologías y Requisitos de Construcción

El proyecto está configurado para compilación cruzada en **entornos Windows (x64)** utilizando **MinGW**.

### Requisitos de Desarrollo

| Componente | Versión / Estándar | Notas |
|------------|------------------|-------|
| Lenguaje | C++17 (`-std=c++17`) | Uso de características modernas del lenguaje. |
| Compilador | g++.exe (MinGW-W64) | Dirigido a arquitectura de 64 bits (`-m64`). |
| Build System | Makefile | Define reglas para limpieza, construcción y ejecución. |
| Dependencias | SDL3, SDL3_image, SDL3_ttf | Librerías de desarrollo multimedia. |

### Comandos de Compilación

```bash
# Compila todo, enlaza el ejecutable y copia las DLLs a output/
make all

# Limpia los directorios obj/ y output/ y reconstruye todo
make fresh

# Compila y ejecuta el programa
make run

# Limpia los archivos generados
make clean
```

🧠 Estructuras de Datos Personalizadas

El núcleo de la aplicación se basa en estructuras de datos implementadas manualmente. No se utilizan contenedores de la STL para el almacenamiento primario; se gestiona de manera explícita la memoria dinámica y los nodos.

1. Listas Enlazadas (datastructures/templates/)
Estructura	Tipo de Enlace	Uso Principal
Lista Doblemente Ligada <T>	Nodos bidireccionales	Contenedor principal de la entidad Receta. Permite recorrido eficiente de inicio a fin y en reversa, necesario para algoritmos como Quicksort.
Lista Simplemente Ligada <T>	Nodos unidireccionales	Estructura anidada para los ingredientes de cada receta.
Lista Circular Doble <T>	Nodos bidireccionales y circulares	Implementación de soporte didáctico.
2. Estructuras Lineales Restringidas

Pila Dinámica (PilaDinamica<T>): Estructura LIFO mediante nodos enlazados dinámicos.

Pila Estática (PilaEstatica<T>): Estructura LIFO con arreglo de tamaño fijo, con control de overflow/underflow.

Cola Dinámica (ColaDinamica<T>): Estructura FIFO mediante nodos enlazados dinámicos.

3. Gestión de Memoria y Regla de los Cinco

Todas las clases de estructuras cumplen los principios de manejo seguro de memoria de C++11/17:

Destructor (~Clase()): Libera la memoria dinámica asignada a los nodos, evitando pérdidas de memoria.

Constructor de copia (Clase(const Clase&)): Copia profunda de la estructura.

Operador de asignación de copia (operator=): Realiza asignación con copia profunda.

Constructor de movimiento (Clase(Clase&&)): Optimiza transferencia de recursos evitando copias innecesarias.

Operador de asignación de movimiento (operator= &&): Optimiza asignación con transferencia de recursos.

📐 Algoritmos Implementados

El manejo de recetas se apoya en algoritmos de ordenamiento y búsqueda implementados manualmente.

1. Algoritmos de Ordenamiento

Quicksort Recursivo: Implementa particionamiento de la lista doblemente ligada y llamadas recursivas a subproblemas. Ordena por:

Nombre de receta

Tiempo de preparación

Categoría

Autor

Mergesort Recursivo: Implementación de apoyo didáctico.

2. Algoritmos de Búsqueda

Búsqueda Binaria (buscarRecetaBinariaPorNombre): Requiere lista ordenada por nombre; complejidad temporal O(log n).

Búsqueda Lineal: Usada en encontrarIndiceRecetaPorNombre para operaciones CRUD y verificación de existencia.

🚨 Módulo de Persistencia (I/O) y Errores Conocidos

El sistema de guardado y carga de datos en formato binario personalizado (.dat) presenta errores críticos que pueden afectar la integridad de los datos:

Serialización: Sobrecarga de operator<< y operator>>.

Flujo de archivo: Uso de std::ofstream y std::ifstream.

Problema Principal: La función cargarRecetasDesdeArchivo puede corromper datos anidados, especialmente listas de ingredientes.

Mitigación Parcial: Validación de elementos, chequeo de eof() y fail(), y validación de integridad de la receta cargada.

Recomendación: Utilizar datos de prueba para evaluar la UI y la lógica de estructuras/algoritmos; evitar depender del guardado/carga para almacenamiento permanente.

🖥️ Interfaz Gráfica (SDL3) y Arquitectura

La capa de presentación utiliza un patrón de estados o pantallas para gestionar vistas.

1. Arquitectura de UI

Clase Game: Inicialización y limpieza de SDL3 y subsistemas.

Clase abstracta GameState: Interfaz común para todas las pantallas (MenuPrincipal, Visualizar, Agregar, etc.) con métodos: procesarEventos(), actualizar(), dibujar().

Clase RutasAssets: Centraliza rutas de recursos (fuentes, iconos, imágenes).

2. Componentes de Interfaz

Botones: Manejan hover y clic mediante delegación de eventos.

Cajas de texto: Entrada de texto mediante eventos de teclado SDL3.

Renderizado de texto: SDL3_ttf y RenderizadorTextos para caching y gestión de fuentes.
