#include "Heart.h"

using namespace std;

Heart::Heart(int X, int Y,SDL_Renderer* renderer){
    x = X;
    y = Y;
    active = true;
    rect = {x ,y, 50, 50};
    heartTexture = IMG_LoadTexture(renderer, "heart.png");
}

void Heart::render(SDL_Renderer* renderer){
    if(active){
        //SDL_SetRenderDrawColor(renderer, 255, 255, 255, 0);
        //SDL_RenderFillRect(renderer, &rect);
        SDL_RenderCopy(renderer, heartTexture, nullptr, &rect);
    }
}
