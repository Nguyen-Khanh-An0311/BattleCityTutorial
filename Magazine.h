#ifndef MAGAZINE_H_INCLUDED
#define MAGAZINE_H_INCLUDED

#include <iostream>
#include "SDL.h"
#include <SDL_image.h>
using namespace std;


class Magazine{
public:
    int x,y;
    SDL_Rect rect;
    SDL_Texture* magazineTexture;
    bool active;

    Magazine(int, int, SDL_Renderer*);

    void render(SDL_Renderer*);
};

#endif // MAGAZINE_H_INCLUDED
