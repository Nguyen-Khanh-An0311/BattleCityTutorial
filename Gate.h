#ifndef GATE_H_INCLUDED
#define GATE_H_INCLUDED
#include <bits/stdc++.h>
#include "SDL.h"
#include "SDL_image.h"
#include "Statistics.h"

const int GATE_FRAME_WIDTH = 160;
const int GATE_FRAME_HEIGHT = 125;
const int GATE_FRAME_COUNT = 4;
const Uint32 GATE_FRAME_DURATION = 150;

class Gate {
public:
    int x, y;
    bool active = false;
    SDL_Texture* texture;
    int currentFrame = 0;
    Uint32 lastFrameTime = SDL_GetTicks();
    Gate(){}

    Gate(SDL_Renderer* renderer) {
        texture = IMG_LoadTexture(renderer, "Image//gate.png");
    }

    void spawn(int x_, int y_) {
        x = x_; // ví dụ tọa độ trung tâm map
        y = y_;
        active = true;
    }

    void render(SDL_Renderer* renderer) {
        Uint32 now = SDL_GetTicks();
        if (now - lastFrameTime >= GATE_FRAME_DURATION) {
            currentFrame = (currentFrame + 1) % FRAME_COUNT;
            lastFrameTime = now;
        }

        SDL_Rect srcRect = {
            currentFrame * GATE_FRAME_WIDTH, //x góc trái trên
            0, //y góc trái trên
            GATE_FRAME_WIDTH, //CR frame
            GATE_FRAME_HEIGHT //CC frame
        };
        SDL_Rect destRect = {x, y, TILE_SIZE * 4, TILE_SIZE * 4};
        SDL_RenderCopy(renderer, texture, &srcRect, &destRect);
    }

    SDL_Rect getRect() {
        return { x, y, TILE_SIZE, TILE_SIZE };
    }
};



#endif // GATE_H_INCLUDED
