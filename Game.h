#include <iostream>
#include <SDL.h>
#include <SDL_mixer.h>
#include <SDL_image.h>
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
    SDL_Texture* backgroundTexture;

    SDL_Texture* loadTexture(const string& , SDL_Renderer*);

    bool running;
    GameState state;

    PlayerTank player1;
    PlayerTank player2;

    int enemyNumber = 7;
    vector<EnemyTank> enemies;
    int heartNumber = 5;
    vector<Heart> hearts;
    vector<Wall> walls;

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
