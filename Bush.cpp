#include "Bush.h"

Bush::Bush(int startX, int startY, SDL_Renderer* renderer){
    x = startX;
    y = startY;
    rect = {x, y, TILE_SIZE, TILE_SIZE};
    bushTexture = IMG_LoadTexture(renderer, "bush.png");
}

void Bush::render(SDL_Renderer* renderer) {
    SDL_RenderCopy(renderer, bushTexture, nullptr, &rect);
}
