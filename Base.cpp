#include "Base.h"

Base::Base(){}

Base::Base(int x_, int y_, SDL_Renderer* renderer){
    x = x_;
    y = y_;
    active = true;
    rect = {x, y, TILE_SIZE * 2, TILE_SIZE * 2};
    baseTexture = IMG_LoadTexture(renderer, "Image//base.png");
    baseDismantledTexture = IMG_LoadTexture(renderer, "Image//base_dismantled.png");
}

void Base::render(SDL_Renderer* renderer){
    if (active) {
        SDL_RenderCopy(renderer, baseTexture, nullptr, &rect);
    }
    else{
        SDL_RenderCopy(renderer, baseDismantledTexture, nullptr, &rect);
        SDL_Delay(200);
    }
}
