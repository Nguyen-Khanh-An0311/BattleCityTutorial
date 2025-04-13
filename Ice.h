#ifndef ICE_H_INCLUDED
#define ICE_H_INCLUDED

#include <iostream>
#include <SDL.h>
#include <vector>
#include "Statistics.h"
#include <SDL_image.h>

class Ice{
public:
    int x,y;
    SDL_Rect rect;
    SDL_Texture* iceTexture;

    Ice();
    Ice(int, int, SDL_Renderer*);
    void render(SDL_Renderer*);
};

#endif // ICE_H_INCLUDED
