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
using namespace std;


enum GameState { PLAYING, WIN, GAME_OVER, MENU, EXIT };
enum GameMode {
    NONE,
    PVE,
    PVP
};


class Game{
public:
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Texture* menuTexture;
    SDL_Texture* winnerTexture;
    SDL_Texture* explosionTexture;
    SDL_Texture* levelTexture;

    SDL_Texture* loadTexture(const string& , SDL_Renderer*);
    TTF_Font* font;
    Mix_Music* backgroundMusic;
    Mix_Chunk* shootSound;


    bool running;
    GameState state;
    GameMode mode;
    void initMode(GameMode);
    int menuSelection = 0; // 0: PVE, 1: PVP
    const int MENU_COUNT = 3;


    PlayerTank player1;
    int scoreP1;
    PlayerTank player2;
    int scoreP2;

    //Boss boss;

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



    Game();
    void run();
    void reset();

    void handleEvents();
    void showMenu();
    void ChooseMode();
    void update();
    void render();
    void renderMenu();
    void renderLevel();
    void renderScore();
    void renderWinner();
    void renderRemainingLive(GameMode);

    //void modeGame(mode);
    void generateWalls();
    void spawnEnemies();
    void spawnHearts();

    int level = 0;

    ~Game();
};
