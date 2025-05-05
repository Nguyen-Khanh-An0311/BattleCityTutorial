#ifndef WALL_H_INCLUDED
#define WALL_H_INCLUDED


#include <iostream>
#include <SDL.h>
#include <vector>
#include "Statistics.h"
#include <SDL_image.h>



class Wall{
public:
    int x,y;
    SDL_Rect rect;
    SDL_Texture* wallTexture;
    bool active;

    Wall();
    Wall(int, int, SDL_Renderer*);
    void render(SDL_Renderer*);
};

#endif // WALL_H_INCLUDED
