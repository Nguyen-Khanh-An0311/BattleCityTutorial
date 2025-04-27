#ifndef ENEMYTANK_H_INCLUDED
#define ENEMYTANK_H_INCLUDED

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

class EnemyTank{
public:
    int x, y;
    int dirX, dirY;
    int angle;
    int moveDelay, shootDelay;
    bool isMoving = true;
    int moveFrames = 0;

    SDL_Rect rect;
    SDL_Texture* tankTexture;
    bool active;
    vector<Bullet> bullets;

    EnemyTank();
    EnemyTank(int, int, SDL_Renderer*);
    bool isBlocked(SDL_Rect rect, vector<Stone>&, vector<Water>&);
    void move(const vector<Wall>&, SDL_Renderer*,vector<Stone>&, vector<Bush>&, vector<Water>&, Base);
    void shoot(SDL_Renderer*);
    void updateBullets();
    void render(SDL_Renderer*);

    int level = 0;
};

#endif // ENEMYTANK_H_INCLUDED
