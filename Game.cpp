#include "Game.h"
using namespace std;

Game::Game(){
            running = true;
            if(SDL_Init(SDL_INIT_EVERYTHING) < 0){
                cout << "SDL could not initialize! SDL_Error: " << SDL_GetError() << endl;
                running = false;
            }

            if (SDL_Init(SDL_INIT_AUDIO) < 0) {
                cout << "SDL_Init Error: " << SDL_GetError() << std::endl;
                running = false;
            }

            if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
                cout << "SDL_image không thể khởi tạo! IMG_Error: " << IMG_GetError() << std::endl;
                running = false;
            }


            window = SDL_CreateWindow(
                "Battle City",
                SDL_WINDOWPOS_CENTERED,
                SDL_WINDOWPOS_CENTERED,
                SCREEN_WIDTH,
                SCREEN_HEIGHT,
                SDL_WINDOW_SHOWN
            );
            if(!window){
                cout << "Window could not be created! SDL_Error: " << SDL_GetError() << endl;
                running = false;
            }

            renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
            if(!renderer){
                cout << "Renderer could not be created! SDL_Erroe: " << SDL_GetError() << endl;
                running = false;
            }

            backgroundTexture = loadTexture("background.png",renderer);
            spawnHearts();
            generateWalls();

            player = PlayerTank(((MAP_WIDTH - 1) / 2) * TILE_SIZE, (MAP_HEIGHT - 2) * TILE_SIZE, renderer);

            spawnEnemies();
            //spawnHearts();
        }


SDL_Texture* Game::loadTexture(const string& path, SDL_Renderer* renderer) {
            SDL_Texture* texture = nullptr;
            SDL_Surface* loadedSurface = IMG_Load(path.c_str());
            if (!loadedSurface) {
                cout << "Không thể tải ảnh! IMG_Error: " << IMG_GetError() << endl;
                return nullptr;
            }

            texture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
            SDL_FreeSurface(loadedSurface);

            if (!texture) {
                cout << "Không thể tạo texture! SDL_Error: " << SDL_GetError() << endl;
            }

            return texture;
        }


void Game::run() {
            if(running){
                handleEvents();
                update();
                render();
                SDL_Delay(16);
            }
        }

void Game::reset() {
    running = true;
    player.RemainingLives = 3;
    state = PLAYING;
    generateWalls();
    player = PlayerTank(((MAP_WIDTH - 1) / 2) * TILE_SIZE, (MAP_HEIGHT - 2) * TILE_SIZE, renderer);
    spawnEnemies();
    spawnHearts();
}


void Game::handleEvents() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {   // THOAT
            running = false;
        } else if (event.type == SDL_KEYDOWN) {
            switch (event.key.keysym.sym) {
                case SDLK_UP:
                    player.move(0, -5, walls, hearts, enemies);
                    player.tankTexture = IMG_LoadTexture(renderer, "player_up.png");
                    break;
                case SDLK_DOWN:
                    player.move(0, 5, walls, hearts, enemies);
                    player.tankTexture = IMG_LoadTexture(renderer, "player_down.png");
                    break;
                case SDLK_LEFT:
                    player.move(-5, 0, walls, hearts, enemies);
                    player.tankTexture = IMG_LoadTexture(renderer, "player_left.png");
                    break;
                case SDLK_RIGHT:
                    player.move(5, 0, walls, hearts, enemies);
                    player.tankTexture = IMG_LoadTexture(renderer, "player_right.png");
                    break;
                case SDLK_SPACE: player.shoot(); break;
            }
        }

        if (state == GAME_OVER && event.type == SDL_KEYDOWN){
            if (event.key.keysym.sym == SDLK_RETURN){
                reset();
            }
        }
    }
}
void Game::render(){
            /*SDL_SetRenderDrawColor(renderer, 128, 128, 128, 255); //chọn màu
            SDL_RenderClear(renderer); // tô toàn bộ màu vừa chọn

            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // chọn màu mới
            for(int i=1; i < MAP_HEIGHT - 1; i++){
                for(int j=1; j < MAP_WIDTH - 1; j++){
                    SDL_Rect tile = {j*TILE_SIZE, i*TILE_SIZE, TILE_SIZE, TILE_SIZE}; // tạo hcn tên tile (x,y,cr,cd)
                    SDL_RenderFillRect(renderer, &tile);
                }
            }*/
            SDL_RenderClear(renderer);
            SDL_RenderCopy(renderer, backgroundTexture, nullptr, nullptr); // Hiển thị ảnh nền


            for (int i=0; i < heartNumber; i++){
                hearts[i].render(renderer);
            }

            for(int i=0; i<walls.size(); i++){
                walls[i].render(renderer);
            }

            player.render(renderer);

            for (auto& enemy : enemies) {
                enemy.render(renderer);
            }

            /*for (int i=0; i < heartNumber; i++){
                hearts[i].render(renderer);
            }*/

            SDL_RenderPresent(renderer); // hiển thị tất cả, phải có
        }





Game::~Game(){
            SDL_DestroyTexture(backgroundTexture);
            IMG_Quit();
            SDL_DestroyRenderer(renderer);
            SDL_DestroyWindow(window);
            SDL_Quit();
        }

void Game::generateWalls(){
    /*for (int i = 3; i < MAP_HEIGHT - 3; i += 2) {
        for (int j = 3; j < MAP_WIDTH - 3; j += 2) {
            Wall w = Wall(j * TILE_SIZE, i * TILE_SIZE);
            walls.push_back(w);
        }
    }*/
    /*Wall w = Wall(2 * TILE_SIZE, 3 * TILE_SIZE);
    walls.push_back(w);*/

    walls.clear();
    for (int i = 0; i < 30; ++i) {
        int wx, wy;
        bool validPosition = false;
        while (!validPosition) {
            wx = (rand() % (MAP_WIDTH - 2) + 1) * TILE_SIZE;
            wy = (rand() % (MAP_HEIGHT - 2) + 1) * TILE_SIZE;
            validPosition = true;


            if (wx == player.x && wy == player.y) {
                validPosition = false;
            }
            break;

            for (const auto& enemy : enemies) {
                if (enemy.active && enemy.x == wx && enemy.y == wy) {
                    validPosition = false;
                    break;
                }
            }
            break;

            for (const auto& heart : hearts) {  // Di chuyển vào vòng while
                if (heart.active && heart.x == wx && heart.y == wy) {
                    validPosition = false;
                    break;
                }
            }
            break;
        }
        walls.push_back(Wall(wx, wy, renderer));
    }
}

void Game::update() {
    player.updateBullets();

    for (auto& enemy : enemies) {
        enemy.move(walls, renderer);
        enemy.updateBullets();
        if (rand() % 100 < 2) {
            enemy.shoot();
        }
    }

    for (auto& enemy : enemies) {
        for (auto& bullet : enemy.bullets) {
            for (auto& wall : walls) {
                if (wall.active && SDL_HasIntersection(&bullet.rect, &wall.rect)) {
                    wall.active = false;
                    bullet.active = false;
                    break;
                }
            }
        }
    }

    for (auto& bullet : player.bullets) {
    for (auto& wall : walls) {
        if (wall.active && SDL_HasIntersection(&bullet.rect, &wall.rect)) {
            wall.active = false;
            bullet.active = false;
            break;
        }
    }
}

for (auto& bullet : player.bullets) {
    for (auto& enemy : enemies) {
        if (enemy.active && SDL_HasIntersection(&bullet.rect, &enemy.rect)) {
            enemy.active = false;
            bullet.active = false;
        }
    }
}

enemies.erase(std::remove_if(enemies.begin(), enemies.end(),
    [](EnemyTank& e) { return !e.active; }), enemies.end());

if (enemies.empty()) {
    state = GAME_OVER;
    //drawText("GAME OVER - PRESS ENTER TO RESTART", 200, 300);
    handleEvents();
}

for (auto& enemy : enemies) {
    for (auto& bullet : enemy.bullets) {
        // Update
        if (SDL_HasIntersection(&bullet.rect, &player.rect)) {
            bullet.active = false;
            player.RemainingLives -= 1;
            if(player.RemainingLives == 0){
                reset();
                return;
            }
        }
    }
}



}

void Game::spawnEnemies() {
    enemies.clear();
    for (int i = 0; i < enemyNumber; ++i) {
        int ex, ey;
        bool validPosition = false;
        while (!validPosition) {
            ex = (rand() % (MAP_WIDTH - 2) + 1) * TILE_SIZE;
            ey = (rand() % (MAP_HEIGHT - 2) + 1) * TILE_SIZE;
            validPosition = true;
            for (const auto& wall : walls) {
                if (wall.active && wall.x == ex && wall.y == ey) {
                    validPosition = false;
                    break;
                }
            }
        }
        enemies.push_back(EnemyTank(ex, ey, renderer));
    }
}

void Game::spawnHearts(){
    hearts.clear();
    for (int i = 0; i < heartNumber; ++i) {
        int hx, hy;
        bool validPosition = false;
        while (!validPosition) {
            hx = (rand() % (MAP_WIDTH - 2) + 1) * TILE_SIZE;
            hy = (rand() % (MAP_HEIGHT - 2) + 1) * TILE_SIZE;
            validPosition = true;
            if (player.x == hx && player.y == hy){
                validPosition = false;
            }
            for (const auto& wall : walls) {
                    for(const auto& enemy : enemies){
                        if ( (wall.active && wall.x == hx && wall.y == hy) ||
                            (enemy.active && enemy.x == hx && enemy.y == hy) ) {
                            validPosition = false;
                            break;
                        }
                    }
            }
            hearts.push_back(Heart(hx, hy, renderer));
        }
    }
}

