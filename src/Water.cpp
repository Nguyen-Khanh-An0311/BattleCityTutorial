#include "Water.h"

Water::Water(int startX, int startY, SDL_Renderer* renderer){
    x = startX;
    y = startY;
    rect = {x, y, TILE_SIZE, TILE_SIZE};
    waterTexture = IMG_LoadTexture(renderer, "Image//water.png");
}

void Water::render(SDL_Renderer* renderer) {
        SDL_RenderCopy(renderer, waterTexture, nullptr, &rect);
}
