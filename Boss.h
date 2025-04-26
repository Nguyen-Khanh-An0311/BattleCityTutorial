#ifndef BOSS_H_INCLUDED
#define BOSS_H_INCLUDED
#include <bits/stdc++.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include "Statistics.h"
#include "Effect.h"
#include "Boss_Juior.h"
#include "Wall.h"
#include "Stone.h"
#include "PlayerTank.h"
using namespace std;

const int FRAME_WIDTH = 64;
const int FRAME_HEIGHT = 64;
const int FRAME_COUNT = 9;
const int DIE_FRAME_COUNT = 9;
const Uint32 FRAME_DURATION = 150;


class Boss {  //base class
public:
    int x, y;
    string name;
    bool active = true;
    int RemainingLives = 10;
    SDL_Texture* texture;

    int currentFrame = 0;
    Uint32 lastFrameTime = SDL_GetTicks();
    SDL_Rect destRect = {x, y, TILE_SIZE * 5.5, TILE_SIZE * 5.5 };

    SDL_Texture* bossDie;
    bool isDying = false;
    int currentFrame_die = 0;
    Uint32 lastFrameTime_die = 0;

    Boss(){}
    Boss(int x, int y) : x(x), y(y) {}
    Mix_Chunk* fireSound;
    Mix_Chunk* bossSound;
    virtual void update() = 0; // hàm ảo để overide
    virtual void render(SDL_Renderer* renderer) = 0;
    virtual bool checkCollision(PlayerTank&) = 0;
    virtual void Die(SDL_Renderer*) = 0;
    virtual ~Boss() {}
};

class FireBoss : public Boss { //derived class
public:
    FireBoss(int x, int y, SDL_Renderer* renderer);
    SDL_Texture* fireTexture;
    vector<unique_ptr<FireZone>> fireZones;
    Uint32 fireInterval = 10000; // khoang nghi
    Uint32 lastFireTime = 0;


    void update() override;
    void render(SDL_Renderer* renderer) override ;
    bool checkCollision(PlayerTank& player) override;
    void Die(SDL_Renderer* renderer) override;
    void spawnFireZone();
};

class IceBoss : public Boss { //derived class
public:
    IceBoss(int x, int y, SDL_Renderer* renderer);
    void update() override;
    void render(SDL_Renderer* renderer) override ;
};


#endif // BOSS_H_INCLUDED
