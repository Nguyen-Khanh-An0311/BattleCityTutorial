#include <iostream>
#include <SDL.h>
#include <vector>
#include "PlayerTank.h"
#include "EnemyTank.h"
using namespace std;

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const int TILE_SIZE = 40;
const int MAP_WIDTH = SCREEN_WIDTH / TILE_SIZE;
const int MAP_HEIGHT = SCREEN_HEIGHT / TILE_SIZE;

class Game{
public:
    SDL_Window* window;
    SDL_Renderer* renderer;
    bool running;
    vector<Wall> walls;
    PlayerTank player;
    int enemyNumber = 3;
    vector<EnemyTank> enemies;

    Game();
    void handleEvents();
    void update();
    void render();
    void run();

    void generateWalls();
    void spawnEnemies();

    ~Game();
};
