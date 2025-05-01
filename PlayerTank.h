#ifndef PLAYERTANK_H_INCLUDED
#define PLAYERTANK_H_INCLUDED


#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <vector>
#include <algorithm>
#include "Bullet.h"
#include "Wall.h"
#include "Statistics.h"
#include "Heart.h"
#include "EnemyTank.h"
#include "Map.h"
using namespace std;

class EnemyTank;

class PlayerTank{
public:
    int x,y;
    SDL_Rect rect;
    int angle;
    const char* imgLink;
    SDL_Texture* tankTexture;

    int dirX, dirY;
    int RemainingLives;
    int cooldown;
    int score;
    bool active;
    vector<Bullet> bullets;

    PlayerTank(int, int, SDL_Renderer*, const char*);
    PlayerTank();

    void move(int, int, const vector<Wall>&, vector<Heart>&, vector<EnemyTank>&, vector<Stone>&, vector<Bush>&, vector<Water>&);
    void shoot(SDL_Renderer*);
    void updateBullets();

    void render(SDL_Renderer*);
};

#endif // TEMP_H_INCLUDED
