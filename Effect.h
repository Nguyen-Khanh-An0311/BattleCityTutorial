#ifndef EFFECT_H_INCLUDED
#define EFFECT_H_INCLUDED
#include <bits/stdc++.h>
#include <SDL.h>
#include <SDL_image.h>
#include "PlayerTank.h"
#include "Statistics.h"
using namespace std;

const int FIRE_FRAME_WIDTH = 80;
const int FIRE_FRAME_HEIGHT = 266;
const int FIRE_FRAME_COUNT = 18;
const Uint32 FIRE_FRAME_DURATION = 100;

class Effect{
public:
    int x, y;
    SDL_Rect rect;
    SDL_Texture* texture;
    int currentFrame = 0;
    Uint32 lastFrameTime = -10000;
    Uint32 spawnTime;
    Uint32 duration = 3000; // Effect tồn tại trong 10 giây

    Effect(){}
    Effect(int x, int y) : x(x), y(y) {}
    virtual void update() = 0; // hàm ảo để overide
    virtual void render(SDL_Renderer* renderer) = 0;
    virtual ~Effect() {}
};

class FireZone : public Effect {
public:
    FireZone(){}
    FireZone(int x, int y, SDL_Texture* tex) : Effect(x, y) {
        texture = tex;
        spawnTime = SDL_GetTicks();
        rect = {
            x,                       // canh giữa theo chiều ngang
            y,
            TILE_SIZE * 2,
            TILE_SIZE * 4
        };
    }

    void update(){}

    void render(SDL_Renderer* renderer) {
        Uint32 now = SDL_GetTicks();
        if (now - lastFrameTime >= FIRE_FRAME_DURATION) {
            currentFrame = (currentFrame + 1) % FIRE_FRAME_COUNT;
            lastFrameTime = now;
        }

        SDL_Rect srcRect = { currentFrame * FIRE_FRAME_WIDTH, 0, FIRE_FRAME_WIDTH, FIRE_FRAME_HEIGHT };
        SDL_RenderCopy(renderer, texture, &srcRect, &rect);
    }

    bool isExpired() {
        return SDL_GetTicks() - spawnTime > duration;
    }
};

#endif // EFFECT_H_INCLUDED
