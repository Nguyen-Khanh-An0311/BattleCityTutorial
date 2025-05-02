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
    SDL_Texture* spawnTexture;
    bool active;
    vector<Bullet> bullets;

    Uint32 spawnStartTime;
    const Uint32 SPAWN_DURATION = 3000;
    Uint32 lastFrameTime;
    int currentFrame;
    bool doneSpawn();

    EnemyTank();
    EnemyTank(const EnemyTank&);
    EnemyTank(int, int, SDL_Renderer*);
    void init(int startX, int startY, SDL_Renderer* renderer);
    EnemyTank& operator=(const EnemyTank& other);

    bool isBlocked(SDL_Rect rect, vector<Stone>&, vector<Water>&);
    void move(vector<Wall>&, SDL_Renderer*,vector<Stone>&, vector<Bush>&, vector<Water>&, Base);
    void shoot(SDL_Renderer*);
    void updateBullets();
    void render(SDL_Renderer*);

    int level = 0;
};

#endif // ENEMYTANK_H_INCLUDED

/*#ifndef ENEMYTANK_H_INCLUDED
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
    EnemyTank(const EnemyTank&);
    EnemyTank& operator=(const EnemyTank&);
    bool isBlocked(SDL_Rect rect, vector<Stone>&, vector<Water>&);
    void move(vector<Wall>&, SDL_Renderer*,vector<Stone>&, vector<Bush>&, vector<Water>&, Base);
    void shoot(SDL_Renderer*);
    void updateBullets();
    void render(SDL_Renderer*);

    int level = 0;
};*/

//#endif // ENEMYTANK_H_INCLUDED
