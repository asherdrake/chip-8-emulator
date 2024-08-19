all:
	g++ -Isrc/include/SDL2 -Lsrc/lib -o chip8 src/main.cpp src/chip8.cpp src/sdldisplay.cpp -lmingw32 -lSDL2main -lSDL2