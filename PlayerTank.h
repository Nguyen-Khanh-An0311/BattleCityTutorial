#ifndef PLAYERTANK_H_INCLUDED
#define PLAYERTANK_H_INCLUDED


#include <iostream>
#include <SDL.h>
#include <vector>
#include <algorithm>
#include "Bullet.h"
#include "Wall.h"
#include "Statistics.h"
using namespace std;

class PlayerTank{
public:
    int x,y;
    SDL_Rect rect;
    int dirX, dirY;
    vector<Bullet> bullets;

    PlayerTank(int, int);
    PlayerTank();

    void move(int, int, const vector<Wall>&);
    void shoot();
    void updateBullets();

    void render(SDL_Renderer*);
};

#endif // TEMP_H_INCLUDED
