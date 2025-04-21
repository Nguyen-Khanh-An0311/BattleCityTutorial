#ifndef EXPLOSION_H_INCLUDED
#define EXPLOSION_H_INCLUDED
#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include "Statistics.h"

class Explosion {
public:
    Explosion(SDL_Renderer*, int x, int y);
    void render(SDL_Renderer* renderer);
    bool isFinished();
    bool soundPlay = true;

    SDL_Texture* explosionTexture;
    SDL_Rect dstRect;
    Uint32 startTime; // thời điểm bắt đầu vụ nổ
    Uint32 duration; // thời gian tồn tại vụ nổ
    Mix_Chunk* explosionSound;
};


#endif // EXPLOSION_H_INCLUDED
