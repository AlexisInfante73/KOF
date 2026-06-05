all:
	g++ src/main.cpp src/combatemusical.cpp src/personaje.cpp src/banda.cpp -o bin/juego.exe -IC:/msys64/mingw64/include -LC:/msys64/mingw64/lib -lsfml-graphics -lsfml-window -lsfml-system

clean:
	rm -f bin/juego.exe