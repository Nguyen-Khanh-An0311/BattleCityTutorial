#ifndef BUSH_H_INCLUDED
#define BUSH_H_INCLUDED

#include <iostream>
#include <SDL.h>
#include <vector>
#include "Statistics.h"
#include <SDL_image.h>

class Bush{
public:
    int x,y;
    SDL_Rect rect;
    SDL_Texture* bushTexture;

    Bush();
    Bush(int, int, SDL_Renderer*);
    void render(SDL_Renderer*);
};


#endif // BUSH_H_INCLUDED
