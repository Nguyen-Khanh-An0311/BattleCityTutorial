#include "Magazine.h"

using namespace std;

Magazine::Magazine(int X, int Y,SDL_Renderer* renderer){
    x = X;
    y = Y;
    active = true;
    rect = {x ,y, 30, 30};
    magazineTexture = IMG_LoadTexture(renderer, "Image//magazine.png");
}

void Magazine::render(SDL_Renderer* renderer){
    if(active){
        //SDL_SetRenderDrawColor(renderer, 255, 255, 255, 0);
        //SDL_RenderFillRect(renderer, &rect);
        SDL_RenderCopy(renderer, magazineTexture, nullptr, &rect);
    }
}
