#include "sdldisplay.h"
#include <cstdio>

SDLDisplay::SDLDisplay(char const *title, int windowWidth, int windowHeight, int textureWidth, int textureHeight)
{
    printf("Initializing SDL.\n");

    /* Initialize defaults, Video and Audio */
    if ((SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0))
    {
        printf("Could not initialize SDL: %s.\n", SDL_GetError());
        exit(-1);
    }

    printf("SDL initialized.\n");

    window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, windowWidth, windowHeight, SDL_WINDOW_RESIZABLE);

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, textureWidth, textureHeight);
};