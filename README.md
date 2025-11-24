# 📚 Recetario Interactivo v2.0.0

### Aplicación de Recetas con Estructuras de Datos Avanzadas en C++ & SDL3

---

## 📌 Resumen Ejecutivo

Recetario Interactivo v2.0.0 es una aplicación de escritorio enfocada en la *gestión eficiente de recetas de cocina, diseñada como una demostración práctica de **estructuras de datos avanzadas implementadas manualmente en C++17*.
A diferencia de aplicaciones comunes, *no utiliza contenedores STL* (como std::vector o std::list).
Toda la lógica de listas, pilas, colas, nodos y memoria dinámica fue desarrollada desde cero para mostrar dominio técnico profundo.

---

# 🚀 Estado Actual (Versión 2.0.0 – Stable Release)

La versión 2.0.0 introduce mejoras sustanciales de rendimiento, estabilidad y experiencia de usuario:

| Módulo                   | Estado        | Detalles Técnicos                                                                                 |
| ------------------------ | ------------- | ------------------------------------------------------------------------------------------------- |
| *Estructuras de Datos* | ✅ Completas   | Implementación manual de listas doblemente ligadas, pilas y colas. Memoria administrada con RAII. |
| *Algoritmos*           | ✅ Optimizados | Quicksort recursivo y búsqueda binaria operando directamente sobre listas enlazadas.              |
| *Interfaz Gráfica*     | ✅ Pulida      | UI basada en SDL3 sin parpadeos, con feedback visual amigable.                                    |
| *Persistencia I/O*     | ✅ Estable     | Nuevo sistema asíncrono de carga/guardado sin congelar la interfaz.                               |
| *Rendimiento*          | ✅ Alto        | Maneja más de *1000 recetas simultáneas* sin pérdida notable de rendimiento.                    |

---

# ✨ Novedades de la Versión 2.0.0

### 🔹 Carga Asíncrona (Multithreading)

Implementada con std::future y std::async.
Permite leer y guardar archivos .dat sin bloquear la interfaz.

### 🔹 Feedback Visual Interactivo

* Animaciones de partículas (lluvia/confeti) al guardar o salir
* Indicadores de carga en tiempo real
* Transiciones suaves entre pantallas

### 🔹 Drag & Drop de Imágenes

Arrastra una imagen desde el explorador directamente a la ventana para asignarla a una receta.

### 🔹 Generación Masiva

Herramienta interna que crea *1000 recetas de prueba* para stress testing de algoritmos.

---

# 🛠 Tecnologías y Requisitos

| Componente       | Tecnología                     | Motivo                                                                   |
| ---------------- | ------------------------------ | ------------------------------------------------------------------------ |
| *Lenguaje*     | C++17                          | Uso de std::filesystem, std::future, semántica de movimiento y RAII. |
| *Compilador*   | g++ (MinGW-W64 – UCRT64/MSYS2) | Flags estrictos: -Wall -Wextra -m64.                                   |
| *Gráficos*     | SDL3                           | Renderizado acelerado, manejo de ventanas y eventos.                     |
| *Extensiones*  | SDL3_image, SDL3_ttf           | Carga de imágenes PNG/JPG y fuentes TTF.                                 |
| *Build System* | Makefile                       | Compilación y despliegue automatizado.                                   |

---

# 🧠 Arquitectura de Datos (Implementación Manual)

Las estructuras están ubicadas en include/datastructures/ y cumplen estrictamente con la *Regla de los Cinco* para evitar fugas de memoria.

## ⿡ Estructuras Lineales

### ✔ Lista Doblemente Ligada (ListaDoblementeLigada<T>)

* Contenedor principal del recetario
* Recorrido bidireccional
* Soporta Quicksort sin copiar datos a arreglos

### ✔ Lista Simplemente Ligada (ListaSimplementeLigada<T>)

* Usada para los ingredientes de cada receta

### ✔ Pila Dinámica (PilaDinamica<T>)

* Manejo del patrón State (navegación entre pantallas)

---

## ⿢ Algoritmos

### ✔ *Quicksort Recursivo*

Adaptado para listas doblemente ligadas.
Criterios de ordenamiento:

* Nombre
* Tiempo
* Categoría
* Autor

### ✔ *Búsqueda Binaria*

Operativa sobre la lista ya ordenada en O(log n).

---

# 🖥 Arquitectura de la Interfaz (UI)

Patrón utilizado: *State Pattern*

### 🔹 Game (Contexto Global)

* Mantiene ventana, renderer y pila de estados.

### 🔹 GameState (Interfaz Base)

Cada pantalla implementa:

* handleEvents()
* update()
* render()

### Pantallas actuales:

* *PantallaMenuPrincipal*
* *PantallaVisualizarRecetas*
* *PantallaAgregarReceta*
* *PantallaGestionArchivos*
* *PantallaExito / PantallaSalida*

---

# ⚙ Compilación y Ejecución

### 📌 Requisitos previos

* MSYS2 (UCRT64)
* SDL3 + SDL3_image + SDL3_ttf instaladas
* Make

### 📦 Comandos (Makefile)

bash
# 1. Limpieza completa y recompilación (Recomendado)
make fresh

# 2. Compilar solo cambios
make

# 3. Ejecutar la aplicación
make run

# 4. Limpiar archivos temporales
make clean


### 🔎 Nota de despliegue

El Makefile copia automáticamente:

* DLLs necesarias (SDL3.dll, libgcc... etc.)
* Carpeta assets/
  al directorio output/, generando un binario portable.

---

# 📂 Estructura del Proyecto


Recetario/
├── assets/             # Fuentes, Imágenes, Iconos, Datos
├── include/
│   ├── datastructures/ # Estructuras de datos personalizadas
│   ├── entities/       # Receta, Ingrediente, Nombre...
│   ├── persistence/    # Lectura y escritura de archivos
│   ├── ui/             # Componentes gráficos
│   └── utils/          # Validadores y funciones auxiliares
├── lib/                # Librerías externas (SDL3)
├── src/                # Código fuente (.cpp)
└── output/             # Ejecutable final


---

# 👨‍💻 Autoría

* *Autor:* [Tu Nombre / Equipo]
* *Curso:* Estructuras de Datos
* *Versión:* 2.0.0 (Stable Release)
