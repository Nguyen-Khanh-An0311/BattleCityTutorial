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
#include "AudioManager.h"
using namespace std;

const int FRAME_WIDTH = 64;
const int FRAME_HEIGHT = 64;
const int FRAME_COUNT = 4;
const int DIE_FRAME_COUNT = 9;
const Uint32 FRAME_DURATION = 200;


class Boss {  //base class
public:
    int x, y;
    string name;
    bool active = true;
    const double fullHP = 15;
    double RemainingLives = fullHP;
    double lastRemainingLives = RemainingLives;
    SDL_Texture* texture;
    SDL_Texture* holeTexture;
    SDL_Texture* shieldTexture;
    Boss(){}
    Boss(int x, int y , SDL_Renderer* renderer) : x(x), y(y) {
        holeTexture = IMG_LoadTexture(renderer, "Image//hole.jpg");
        shieldTexture = IMG_LoadTexture(renderer, "Image//shield.png");
        AudioManager::Init();
    }

    //hiển thị
    int currentFrame = 0;
    Uint32 lastFrameTime = SDL_GetTicks();
    SDL_Rect destRect = {x, y, TILE_SIZE * 6, TILE_SIZE * 6 };

    virtual void update() = 0;
    virtual void render(SDL_Renderer* renderer) = 0;

    //hiệu ứng
    virtual bool checkCollision(PlayerTank&) = 0;

    //hiện máu
    void renderHP(SDL_Renderer* renderer);

    //hiện cổng
    Uint32 lastOpenTime;
    Hole* hole = NULL;
    //Hole* spawnHole(SDL_Texture*);
    Hole* spawnHole(SDL_Texture* holeTexture);
    vector<EnemyTank> enemiesFromHole;
    Uint32 holeInterval = 15000;

    //hiện khiên
    Shield* shield = NULL;
    Shield* spawnShield(SDL_Texture* shieldTexture);
    Uint32 lastOpenShieldTime;
    Uint32 shieldInterval = 5000;

    SDL_Texture* bossDie;
    bool isDying = false;
    int currentFrame_die = 0;
    Uint32 lastFrameTime_die = 0;
    virtual void Die(SDL_Renderer*) = 0;

    virtual ~Boss() {}
};

class FireBoss : public Boss { //derived class
public:
    FireBoss(int x, int y, SDL_Renderer* renderer);
    SDL_Texture* fireTexture;
    Mix_Chunk* fireSound;

    //hiện lửa
    vector<unique_ptr<FireZone>> fireZones;
    Uint32 fireInterval = 7000; //khoảng nghỉ
    Uint32 lastFireTime;
    void spawnFireZone();

    //render, update
    void update() override;
    void render(SDL_Renderer* renderer) override ;

    // ktra va chạm
    bool checkCollision(PlayerTank& player) override;

    //chết
    void Die(SDL_Renderer* renderer) override;
};

class IceBoss : public Boss { //derived class
public:
    IceBoss(int x, int y, SDL_Renderer* renderer);
    SDL_Texture* iceTexture;

    vector<unique_ptr<IceZone>> iceZones;
    Uint32 iceInterval = 10000;
    Uint32 lastIceTime = 0;
    void spawnIceZone();

    void update() override;
    void render(SDL_Renderer* renderer) override ;

    bool checkCollision(PlayerTank& player) override;

    void Die(SDL_Renderer*) override;
};


#endif // BOSS_H_INCLUDED
