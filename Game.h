#include <iostream>
#include <SDL.h>
#include <SDL_mixer.h>
#include <vector>
#include "PlayerTank.h"
#include "EnemyTank.h"
#include "Statistics.h"
#include "Heart.h"
using namespace std;


enum GameState { PLAYING, GAME_OVER };

class Game{
public:
    SDL_Window* window;
    SDL_Renderer* renderer;
    Mix_Music* bgm;


    bool running;
    GameState state;

    vector<Wall> walls;
    PlayerTank player;
    int enemyNumber = 30;
    vector<EnemyTank> enemies;
    int heartNumber = 5;
    vector<Heart> hearts;

    Game();
    void run();
    void reset();

    void handleEvents();
    void update();
    void render();



    void generateWalls();
    void spawnEnemies();
    void spawnHearts();

    ~Game();
};
