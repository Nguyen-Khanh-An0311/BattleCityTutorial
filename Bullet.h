#ifndef BULLET_H_INCLUDED
#define BULLET_H_INCLUDED


#include <iostream>
#include <SDL.h>
#include "Statistics.h"


using namespace std;

class Bullet{
public:
    int x,y;
    int dx, dy;
    bool active;
    SDL_Rect rect;

    Bullet(int, int, int, int);
    void move();
    void render(SDL_Renderer*);
};

#endif // BULLET_H_INCLUDED
