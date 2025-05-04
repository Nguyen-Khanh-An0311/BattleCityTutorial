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

const int SPAWN_FRAME_WIDTH = 32;
const int SPAWN_FRAME_HEIGHT = 32;
const int SPAWN_FRAME_COUNT = 10;
const Uint32 SPAWN_FRAME_DURATION = 100;

enum State {NORMAL, FROZEN};

class PlayerTank{
public:
    int x,y;
    SDL_Rect rect;
    int angle;
    const char* imgLink;
    const char* spawnLink;
    SDL_Texture* tankTexture;
    SDL_Texture* spawnTexture;

    int dirX, dirY;
    const int fullHP = 3;
    int RemainingLives;
    int cooldown;
    int score = 0;
    int feat = 0;
    bool active;
    State state;
    Uint32 frozenTime;
    vector<Bullet> bullets;

    PlayerTank(int, int, SDL_Renderer*, const char*);
    PlayerTank();
    void init(int , int , SDL_Renderer* , const char*, const char*);

    //spawn
    Uint32 spawnStartTime;
    const Uint32 SPAWN_DURATION = 3000;
    const Uint32 FROZEN_DURATION = 4000;
    Uint32 lastFrameTime;
    int currentFrame;
    bool doneSpawn();
    bool doneFrozen();

    Uint32 lastShotTime = 0;
    int shootDelay = 150; // đơn vị: milliseconds (ms)


    void move(int, int, const vector<Wall>&, vector<Heart>&, vector<EnemyTank>&, vector<Stone>&, vector<Bush>&, vector<Water>&);
    void shoot(SDL_Renderer*);
    void updateBullets();

    void render(SDL_Renderer*);
    void renderHP(SDL_Renderer* renderer);
};

#endif // TEMP_H_INCLUDED
