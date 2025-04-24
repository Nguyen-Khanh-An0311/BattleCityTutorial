#ifndef BOSS_H_INCLUDED
#define BOSS_H_INCLUDED
#include <bits/stdc++.h>
#include <SDL.h>
#include <SDL_image.h>
#include "Statistics.h"
using namespace std;

const int FRAME_WIDTH = 64;
const int FRAME_HEIGHT = 64;
const int FRAME_COUNT = 9;
const Uint32 FRAME_DURATION = 150;


class Boss {
public:
    int x, y;
    string name;
    bool active = true;
    SDL_Texture* texture;
    int currentFrame = 0;
    Uint32 lastFrameTime = SDL_GetTicks();
    SDL_Rect destRect = {x, y, TILE_SIZE * 5, TILE_SIZE * 5 };

    Boss(int x, int y) : x(x), y(y) {}
    virtual void update() = 0; // hàm ảo để overide
    virtual void render(SDL_Renderer* renderer) = 0;
    virtual ~Boss() {}
};

class FireBoss : public Boss {
public:
    FireBoss(int x, int y, SDL_Renderer* renderer);
    void update() override;
    void render(SDL_Renderer* renderer) override ;
};


#endif // BOSS_H_INCLUDED
