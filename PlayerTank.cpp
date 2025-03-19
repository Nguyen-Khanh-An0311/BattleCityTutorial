#include <iostream>
#include <SDL.h>
#include "PlayerTank.h"
using namespace std;



void PlayerTank::render(SDL_Renderer* renderer){
    SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
    SDL_RenderFillRect(renderer, &rect);
}
