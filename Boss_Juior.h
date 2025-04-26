#ifndef BOSS_JUIOR_H_INCLUDED
#define BOSS_JUIOR_H_INCLUDED

#include <iostream>
#include <SDL_image.h>
#include <vector>
#include <algorithm>
#include "Bullet.h"
#include "Wall.h"
#include "Statistics.h"
#include "Map.h"
#include "PlayerTank.h"
#include "Base.h"
using namespace std;

class PlayerTank;

const Uint32 BJR_FRAME_DURATION = 1500;

/*class BossJR{
public:
    int x, y;
    int dirX, dirY;
    int angle;
    int moveDelay;
    SDL_Rect rect;
    SDL_Texture* bossJRTexture;
    bool active;

    int currentFrame = 0;
    Uint32 lastFrameTime = SDL_GetTicks();

    int F_FRAME_WIDTH = 80;
    int F_FRAME_HEIGHT = 200;
    int F_FRAME_WIDTH = 100;
    int F_FRAME_COUNT = 10;

    BossJR(int, int, SDL_Renderer*);
    void move(const vector<Wall>&, SDL_Renderer*,vector<Stone>&, vector<Bush>&, vector<Water>&, Base);
    void render(SDL_Renderer*);

    int level = 0;
};*/

#endif // BOSS_JUIOR_H_INCLUDED
