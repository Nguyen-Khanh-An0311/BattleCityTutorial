#include "Stone.h"

Stone::Stone(int startX, int startY, SDL_Renderer* renderer){
    x = startX;
    y = startY;
    rect = {x, y, TILE_SIZE, TILE_SIZE};
    stoneTexture = IMG_LoadTexture(renderer, "stone.png");
}

void Stone::render(SDL_Renderer* renderer) {
        SDL_RenderCopy(renderer, stoneTexture, nullptr, &rect);
}
