# Rutas de MSYS2 MinGW64 en tu sistema
INCLUDES = -IC:/msys64/mingw64/include
LIBS = -LC:/msys64/mingw64/lib -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio

# Archivos fuente
SOURCES = src/main.cpp src/combatemusical.cpp src/banda.cpp src/personaje.cpp

# Regla principal para compilar el juego
all: bin/juego

bin/juego: $(SOURCES)
	g++ -B. -Wall -std=c++17 $(INCLUDES) $(SOURCES) -o bin/juego $(LIBS)

# Regla para limpiar el ejecutable viejo
clean:
	rm -f bin/juego bin/juego.exe