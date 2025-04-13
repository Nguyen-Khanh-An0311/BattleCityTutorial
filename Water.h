#ifndef WATER_H_INCLUDED
#define WATER_H_INCLUDED

#include <iostream>
#include <SDL.h>
#include <vector>
#include "Statistics.h"
#include <SDL_image.h>

class Water{
public:
    int x,y;
    SDL_Rect rect;
    SDL_Texture* waterTexture;

    Water();
    Water(int, int, SDL_Renderer*);
    void render(SDL_Renderer*);
};

#endif // WATER_H_INCLUDED
