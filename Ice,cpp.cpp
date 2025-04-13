#include "Ice.h"

Ice::Ice(int startX, int startY, SDL_Renderer* renderer){
    x = startX;
    y = startY;
    rect = {x, y, TILE_SIZE, TILE_SIZE};
    iceTexture = IMG_LoadTexture(renderer, "ice.png");
}

void Ice::render(SDL_Renderer* renderer) {
        SDL_RenderCopy(renderer, iceTexture, nullptr, &rect);
}
