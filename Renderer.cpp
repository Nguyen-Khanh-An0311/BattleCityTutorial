#include "Renderer.h"


window = SDL_CreateWindow(
                "Battle City",
                SDL_WINDOWPOS_CENTERED,
                SDL_WINDOWPOS_CENTERED,
                SCREEN_WIDTH,
                SCREEN_HEIGHT,
                SDL_WINDOW_SHOWN
            );
renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
