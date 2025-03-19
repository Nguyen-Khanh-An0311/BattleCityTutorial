#ifndef WALL_H_INCLUDED
#define WALL_H_INCLUDED




#include <iostream>
#include <SDL.h>
#include <vector>



class Wall{
public:
    int x,y;
    SDL_Rect rect;
    bool active;

    Wall();
    Wall(int, int);
    void render(SDL_Renderer*);
};

#endif // WALL_H_INCLUDED
