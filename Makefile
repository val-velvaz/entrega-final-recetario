# --- CONFIGURACIÓN DEL COMPILADOR ---
CXX = g++.exe

# --- CONFIGURACIÓN DEL PROYECTO ---
TARGET = output/recetario_game.exe

# --- CONFIGURACIÓN DE LIBRERÍAS ---
SDL3_DIR = lib/SDL3/x86_64-w64-mingw32
SDL_TTF_DIR = lib/SDL_ttf/x86_64-w64-mingw32
SDL3_image_DIR = lib/SDL3_image/x86_64-w64-mingw32

INCLUDES = -Iinclude -I$(SDL3_DIR)/include -I$(SDL_TTF_DIR)/include -I$(SDL3_image_DIR)/include
LIBS = -L$(SDL3_DIR)/lib -L$(SDL_TTF_DIR)/lib -L$(SDL3_image_DIR)/lib -lSDL3_image -lSDL3_ttf -lSDL3

# --- BANDERAS (FLAGS) ---
CXXFLAGS = -std=c++14 $(INCLUDES) -m64 -Wall -Wextra
LDFLAGS = $(LIBS)

# --- BÚSQUEDA RECURSIVA DE ARCHIVOS ---
SOURCES = $(shell find src -name '*.cpp')
OBJECTS = $(SOURCES:.cpp=.o)
HEADERS = $(shell find include -name '*.hpp')

# --- REGLAS PRINCIPALES ---
.PHONY: all clean run force rebuild check info

all: $(TARGET)

$(TARGET): $(OBJECTS)
	@echo "=== ENLAZANDO EJECUTABLE ==="
	$(CXX) -o $(TARGET) $(OBJECTS) $(LDFLAGS)
	@echo "COMPILACION EXITOSA: $(TARGET)"
	@ls -lh $(TARGET)

%.o: %.cpp
	@echo "Compilando: $<"
	$(CXX) -c $(CXXFLAGS) $< -o $@

clean:
	@echo "Limpiando..."
	rm -f $(OBJECTS) $(TARGET)
	@echo "Limpieza completa"

force: clean all

rebuild:
	@echo "Recompilando archivos modificados..."
	$(MAKE) all

run: all
	@echo "Ejecutando programa..."
	@echo "========================================"
	./$(TARGET)

check:
	@echo "Archivos fuente:"
	@ls -lh $(SOURCES) | tail -5
	@echo ""
	@echo "Archivos objeto:"
	@ls -lh $(OBJECTS) | tail -5
	@echo ""
	@echo "Ejecutable:"
	@ls -lh $(TARGET) || echo "No existe"

info:
	@echo "Compilador: $(CXX)"
	@echo "Target: $(TARGET)"
	@echo "Archivos fuente: $(words $(SOURCES))"
	@echo "Flags: $(CXXFLAGS)"
	@echo ""
	@find src -name "*.cpp" | head -10
