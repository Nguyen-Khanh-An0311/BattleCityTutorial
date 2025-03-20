#include "Heart.h"

using namespace std;

Heart::Heart(int X, int Y){
    x = X;
    y = Y;
    active = true;
    rect = {x ,y, 10, 10};
}

void Heart::render(SDL_Renderer* renderer){
    if(active){
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 0);
        SDL_RenderFillRect(renderer, &rect);
    }
}
