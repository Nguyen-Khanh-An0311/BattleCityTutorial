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
    SDL_Rect rect;
    SDL_Texture* tankTexture;
    bool active;
    vector<Bullet> bullets;

    EnemyTank(int, int, SDL_Renderer*);
    void move(const vector<Wall>&, SDL_Renderer*,vector<Stone>&, vector<Bush>&, vector<Water>&, Base);
    void shoot(SDL_Renderer*);
    void updateBullets();
    void render(SDL_Renderer*);

    int level = 0;
};

#endif // ENEMYTANK_H_INCLUDED
