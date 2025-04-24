#ifndef GATE_H_INCLUDED
#define GATE_H_INCLUDED
#include <bits/stdc++.h>
#include "SDL.h"
#include "SDL_image.h"
#include "Statistics.h"

class Gate {
public:
    int x, y;
    bool active = false;
    SDL_Texture* texture;

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
        if (!active) return;
        SDL_Rect dst = { x, y, TILE_SIZE * 2, TILE_SIZE * 2 };
        SDL_RenderCopy(renderer, texture, NULL, &dst);
    }

    SDL_Rect getRect() {
        return { x, y, TILE_SIZE, TILE_SIZE };
    }
};



#endif // GATE_H_INCLUDED
