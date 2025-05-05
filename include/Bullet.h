#ifndef BULLET_H_INCLUDED
#define BULLET_H_INCLUDED


#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include "Statistics.h"


using namespace std;

class Bullet{
public:
    int x,y;
    int dx, dy;
    int angle;
    bool active;
    SDL_Rect rect;
    SDL_Texture* bulletTexture;

    Bullet(int, int, int, int, SDL_Renderer*);
    void move();
    void render(SDL_Renderer*);
};

#endif // BULLET_H_INCLUDED
