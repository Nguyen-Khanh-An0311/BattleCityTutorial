#include <iostream>
#include <SDL.h>
using namespace std;

class PlayerTank{
    int x,y;
    SDL_Rect rect;
    int dirX, dirY;

    PlayerTank(int, int);

    void move();
    void render(SDL_Renderer*);
};
