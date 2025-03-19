#include <iostream>
#include <vector>
#include <algorithm>
#include "Bullet.h"
#include "Wall.h"
using namespace std;

class EnemyTank{
public:
    int x, y;
    int dirX, dirY;
    int moveDelay, shootDelay;
    SDL_Rect rect;
    bool active;
    vector<Bullet> bullets;

    EnemyTank(int, int);
    void move(const vector<Wall>&);
    void shoot();
    void updateBullets();
    void render(SDL_Renderer*);
};
