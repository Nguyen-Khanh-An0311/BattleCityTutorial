#ifndef BOSS_H_INCLUDED
#define BOSS_H_INCLUDED
#include <bits/stdc++.h>
#include <SDL.h>
#include <SDL_image.h>
#include "Statistics.h"
using namespace std;

const int FRAME_WIDTH = 64;
const int FRAME_HEIGHT = 64;
const int FRAME_COUNT = 4;
const Uint32 FRAME_DURATION = 150;


class Boss {  //base class
public:
    int x, y;
    string name;
    bool active = true;
    SDL_Texture* texture;
    int currentFrame = 0;
    Uint32 lastFrameTime = SDL_GetTicks();
    SDL_Rect destRect = {x, y, TILE_SIZE * 5.5, TILE_SIZE * 5.5 };

    Boss(){}
    Boss(int x, int y) : x(x), y(y) {}
    virtual void update() = 0; // hàm ảo để overide
    virtual void render(SDL_Renderer* renderer) = 0;
    virtual ~Boss() {}
};

class FireZone {
public:
    SDL_Rect rect;
    SDL_Texture* texture;
    int currentFrame = 0;
    Uint32 lastFrameTime = SDL_GetTicks();
    Uint32 spawnTime;
    Uint32 duration = 3000; // Lửa tồn tại trong 3 giây

    FireZone(int x, int y) : spawnTime(SDL_GetTicks()) {
        rect = {x, y, TILE_SIZE, TILE_SIZE}; // Kích thước vùng lửa
    }

    void render(SDL_Renderer* renderer) {
        texture = IMG_LoadTexture(renderer, "Image//flame.png");

        Uint32 now = SDL_GetTicks();
        if (now - lastFrameTime >= duration) {
            currentFrame = (currentFrame + 1) % 18;
            lastFrameTime = now;
        }
        SDL_Rect srcRect = { currentFrame * 74, 0, 74, 266 };
        SDL_RenderCopy(renderer, texture, &srcRect, &rect);
    }

    bool isExpired() {
        return SDL_GetTicks() - spawnTime > duration;
    }

    bool checkCollision(const SDL_Rect& playerRect) {
        return SDL_HasIntersection(&rect, &playerRect);
    }
};

class FireBoss : public Boss { //derived class
public:
    FireBoss(int x, int y, SDL_Renderer* renderer);
    vector<unique_ptr<FireZone>> fireZones;
    Uint32 fireInterval = 10000;
    Uint32 lastFireTime = 0;


    void update() override;
    void render(SDL_Renderer* renderer) override ;
    void spawnFireZone();
};


#endif // BOSS_H_INCLUDED
