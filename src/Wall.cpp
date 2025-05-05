#include "Wall.h"
using namespace std;


Wall::Wall(int startX, int startY, SDL_Renderer* renderer){
    x = startX;
    y = startY;
    active = true;
    rect = {x, y, TILE_SIZE, TILE_SIZE};
    wallTexture = IMG_LoadTexture(renderer, "Image//wall.png");
}

void Wall::render(SDL_Renderer* renderer) {
    if (active) {
        //SDL_SetRenderDrawColor(renderer, 150, 75, 0, 255); // Brown color
        //SDL_RenderFillRect(renderer, &rect);
        SDL_RenderCopy(renderer, wallTexture, nullptr, &rect);
    }
}

