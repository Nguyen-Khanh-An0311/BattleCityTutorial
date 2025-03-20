#ifndef HEART_H_INCLUDED
#define HEART_H_INCLUDED

#include <iostream>
#include "SDL.h"
using namespace std;


class Heart{
public:
    int x,y;
    SDL_Rect rect;
    bool active;

    Heart(int, int);

    void render(SDL_Renderer*);
};

#endif // HEART_H_INCLUDED
