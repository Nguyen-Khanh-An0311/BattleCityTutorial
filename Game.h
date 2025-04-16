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


enum GameState { PLAYING, WIN, GAME_OVER, MENU };
enum class GameMode {
    PVE,
    PVP
};


class Game{
public:
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Texture* menuTexture;
    SDL_Texture* explosionTexture;

    SDL_Texture* loadTexture(const string& , SDL_Renderer*);
    TTF_Font* font;
    Mix_Music* backgroundMusic;

    bool running;
    GameState state;
    GameMode mode;
    void initMode(GameMode);

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
    void update();
    void render();
    void renderMenu();
    void renderLevel();
    void renderScore();

    //void modeGame(mode);
    void generateWalls();
    void spawnEnemies();
    void spawnHearts();

    int level = 0;

    ~Game();
};
