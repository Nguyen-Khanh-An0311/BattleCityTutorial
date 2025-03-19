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
