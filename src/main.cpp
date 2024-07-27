#include "chip8.h"
#include "sdldisplay.h"

int main(int argc, char **argv)
{
    Chip8 chip8;
    SDLDisplay display("Chip8 Emulator", 640, 320, 64, 32);

    return 0;
}