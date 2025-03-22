#ifndef HEART_H_INCLUDED
#define HEART_H_INCLUDED

#include <iostream>
#include "SDL.h"
#include <SDL_image.h>
using namespace std;


class Heart{
public:
    int x,y;
    SDL_Rect rect;
    SDL_Texture* heartTexture;
    bool active;

    Heart(int, int, SDL_Renderer*);

    void render(SDL_Renderer*);
};

#endif // HEART_H_INCLUDED
