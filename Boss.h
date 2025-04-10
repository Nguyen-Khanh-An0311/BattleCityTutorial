#ifndef BOSS_H_INCLUDED
#define BOSS_H_INCLUDED

#include <iostream>
#include <SDL_image.h>
#include <vector>
#include <algorithm>
#include "Bullet.h"
#include "Wall.h"
#include "Statistics.h"
using namespace std;

class Boss{
public:
    int x, y;
    int dirX, dirY;
    int moveDelay, shootDelay;
    SDL_Rect rect;
    SDL_Texture* tankTexture;
    bool active;
    vector<Bullet> bullets;

    Boss();
    Boss(int, int, SDL_Renderer*);
    void move(const vector<Wall>&, SDL_Renderer*);
    void shoot();
    void updateBullets();
    void render(SDL_Renderer*);
};


#endif // BOSS_H_INCLUDED
