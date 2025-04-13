#ifndef STONE_H_INCLUDED
#define STONE_H_INCLUDED

#include <iostream>
#include <SDL.h>
#include <vector>
#include "Statistics.h"
#include <SDL_image.h>

class Stone{
public:
    int x,y;
    SDL_Rect rect;
    SDL_Texture* stoneTexture;

    Stone();
    Stone(int, int, SDL_Renderer*);
    void render(SDL_Renderer*);
};



#endif // STONE_H_INCLUDED
