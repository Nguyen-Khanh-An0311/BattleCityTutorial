// Explosion.cpp
#include "Explosion.h"
#include <SDL_timer.h>

Explosion::Explosion(SDL_Renderer* renderer, int x, int y) : startTime(SDL_GetTicks()), duration(500) {
    explosionTexture = IMG_LoadTexture(renderer, "Image//explosion.png");
    explosionSound = Mix_LoadWAV("Sound//explosionSound.wav");
    dstRect.x = x;
    dstRect.y = y;
    dstRect.w = TILE_SIZE;
    dstRect.h = TILE_SIZE;
}

void Explosion::render(SDL_Renderer* renderer) {
    if (!isFinished()) {
        SDL_RenderCopy(renderer, explosionTexture, nullptr, &dstRect);
    }
}

bool Explosion::isFinished() {
    return SDL_GetTicks() - startTime >= duration;
}
