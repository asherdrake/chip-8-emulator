#ifndef SDLDISPLAY_H
#define SDLDISPLAY_H

#include <SDL.h>

class SDLDisplay
{
public:
    SDLDisplay(char const *title, int windowWidth, int windowHeight, int textureWidth, int textureHeight);

private:
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Texture *texture;
};

#endif