#include <bits/stdc++.h>
#include <memory>
#include <iostream>
#include <SDL.h>
#include <SDL_mixer.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <vector>
#include "PlayerTank.h"
#include "EnemyTank.h"
#include "Statistics.h"
#include "Heart.h"
#include "Map.h"
#include "Boss.h"
#include "Explosion.h"
#include "Boss.h"
#include "Base.h"
#include "Gate.h"
using namespace std;


enum GameState { PLAYING, WIN, GAME_OVER, MENU, EXIT, SHOW_WINNER };
enum GameMode {
    NONE,
    PVE,
    PVP
};


class Game{
public:
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Texture* loadTexture(const string& , SDL_Renderer*);

    Game();
    GameState state;
    GameMode mode;
    int level = 0;
    ~Game();

    //run
    void run();
    bool running;

    //handle event
    void handleEvents();
    void showMenu();
    void ChooseMode();
    void renderInstruction();
    int menuSelection = 0; // 0: PVE, 1: PVP
    const int MENU_COUNT = 3;

    //update
    void update();

    //render
    void render();
    void renderMenu();
    void renderLevel();
    void renderScore();
    void renderWinner();
    void renderHeart();
    void renderRemainingLive(GameMode);
    SDL_Texture* menuTexture;
    SDL_Texture* winnerTexture;
    SDL_Texture* levelTexture;
    SDL_Texture* RML1;
    SDL_Texture* RML2;
    TTF_Font* font;
    TTF_Font* fontScore;
    Mix_Music* backgroundMusic;
    Mix_Chunk* shootSound;

    //init
    void initMode(GameMode);
    void generateWalls();
    void spawnEnemies();
    void spawnHearts();
    void spawnBoss();
    Base base;
    Gate gateOut;
    PlayerTank player1;
    int scoreP1;
    PlayerTank player2;
    int scoreP2;
    int enemyNumber = 1;
    vector<EnemyTank> enemies;
    int heartNumber = 5;
    vector<Heart> hearts;
    Map gameMap;
    vector<Wall> walls;
    vector<Water> waters;
    vector<Bush> bushs;
    vector<Stone> stones;
    vector<Ice> ices;
    vector<Explosion> explosions;
    unique_ptr<Boss> currentBoss;
};
