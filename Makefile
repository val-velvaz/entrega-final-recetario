# --- CONFIGURACIÓN DEL COMPILADOR ---
CXX = g++.exe

# --- CONFIGURACIÓN DE DIRECTORIOS ---
SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = output
LIB_DIR = lib

# --- CONFIGURACIÓN DEL PROYECTO ---
TARGET = $(BIN_DIR)/recetario_game.exe

# --- CONFIGURACIÓN DE LIBRERÍAS ---
SDL3_PATH = $(LIB_DIR)/SDL3/x86_64-w64-mingw32
SDL_TTF_PATH = $(LIB_DIR)/SDL_ttf/x86_64-w64-mingw32
SDL_IMG_PATH = $(LIB_DIR)/SDL3_image/x86_64-w64-mingw32

INCLUDES = -Iinclude -I$(SDL3_PATH)/include -I$(SDL_TTF_PATH)/include -I$(SDL_IMG_PATH)/include
LIBS = -L$(SDL3_PATH)/lib -L$(SDL_TTF_PATH)/lib -L$(SDL_IMG_PATH)/lib -lSDL3_image -lSDL3_ttf -lSDL3

# --- BANDERAS (FLAGS) ---
CXXFLAGS = -std=c++17 $(INCLUDES) -m64 -Wall -Wextra -g

# --- BÚSQUEDA RECURSIVA DE ARCHIVOS ---
# Nota: En Windows MSYS2, 'find' funciona bien. Si falla, usaremos wildcard.
SOURCES = $(shell find $(SRC_DIR) -name '*.cpp')
OBJECTS = $(SOURCES:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

# --- REGLAS PRINCIPALES ---
.PHONY: all clean fresh run directories

all: directories $(TARGET)

$(TARGET): $(OBJECTS)
	@echo "=== ENLAZANDO EJECUTABLE ==="
	$(CXX) -o $@ $^ $(LIBS)
	@echo "COMPILACION EXITOSA: $@"
	@echo "Copiando DLLs necesarias..."
	@cp $(SDL3_PATH)/bin/*.dll $(BIN_DIR)/ 2>/dev/null || true
	@cp $(SDL_TTF_PATH)/bin/*.dll $(BIN_DIR)/ 2>/dev/null || true
	@cp $(SDL_IMG_PATH)/bin/*.dll $(BIN_DIR)/ 2>/dev/null || true

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	@echo "Compilando: $<"
	$(CXX) -c $(CXXFLAGS) $< -o $@

directories:
	@mkdir -p $(OBJ_DIR)
	@mkdir -p $(BIN_DIR)

clean:
	@echo "Limpiando..."
	rm -rf $(OBJ_DIR) $(BIN_DIR)
	@echo "Limpieza completa."

fresh: clean all

run: all
	@echo "Ejecutando programa..."
	@echo "========================================"
	./$(TARGET)
