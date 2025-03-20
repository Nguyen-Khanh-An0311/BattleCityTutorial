#include <iostream>
#include <SDL.h>
#include <vector>
#include "PlayerTank.h"
#include "EnemyTank.h"
#include "Statistics.h"
using namespace std;


enum GameState { PLAYING, GAME_OVER };

class Game{
public:
    SDL_Window* window;
    SDL_Renderer* renderer;

    bool running;
    GameState state;

    vector<Wall> walls;
    PlayerTank player;
    int enemyNumber = 3;
    vector<EnemyTank> enemies;

    Game();
    void run();
    void reset();

    void handleEvents();
    void update();
    void render();



    void generateWalls();
    void spawnEnemies();

    ~Game();
};
