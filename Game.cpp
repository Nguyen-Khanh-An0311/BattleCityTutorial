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

            player1 = PlayerTank( TILE_SIZE, (MAP_HEIGHT - 2) * TILE_SIZE, renderer);
            player2 = PlayerTank( (MAP_WIDTH - 2) * TILE_SIZE, TILE_SIZE, renderer);
            spawnHearts();

            gameMap.loadFromFile("gameMaps//map1.txt", renderer);
            walls = gameMap.walls;
            spawnEnemies();


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
    player1.RemainingLives = 3;
    player2.RemainingLives = 3;
    state = MENU;
    gameMap.loadFromFile("map1.txt", renderer);
    player1 = PlayerTank( TILE_SIZE, (MAP_HEIGHT - 2) * TILE_SIZE, renderer);
    player2 = PlayerTank( (MAP_WIDTH - 2) * TILE_SIZE, TILE_SIZE, renderer);
    spawnEnemies();
    spawnHearts();
}


void Game::handleEvents() {
        SDL_Event event;
        const Uint8* keystate = SDL_GetKeyboardState(NULL); // Lấy trạng thái bàn phím

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
            else if (state == MENU) {
                if (event.type == SDL_KEYDOWN) {
                    switch (event.key.keysym.sym) {
                        case SDLK_1:
                            mode = GameMode::PVP;
                            state = PLAYING;
                            break;
                        case SDLK_2:
                            mode = GameMode::COOP_BOSS;
                            state = PLAYING;
                            break;
                        default:
                            running = false;
                            break;
                        /*case SDLK_ESCAPE:
                            running = false;
                            break;*/
                    }
                }  // 👈 xử lý menu ở đây
            }
            else if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_RETURN) {
                state = GAME_OVER;
                reset();
            }
            else if(event.type == SDL_KEYDOWN){
                // Điều khiển Player 1 (Phím WASD)
                if (keystate[SDL_SCANCODE_W]) {
                    player1.move(0, -5, walls, hearts, enemies);
                    player1.tankTexture = IMG_LoadTexture(renderer, "player_up.png");
                }
                if (keystate[SDL_SCANCODE_S]) {
                    player1.move(0, 5, walls, hearts, enemies);
                    player1.tankTexture = IMG_LoadTexture(renderer, "player_down.png");
                }
                if (keystate[SDL_SCANCODE_A]) {
                    player1.move(-5, 0, walls, hearts, enemies);
                    player1.tankTexture = IMG_LoadTexture(renderer, "player_left.png");
                }
                if (keystate[SDL_SCANCODE_D]) {
                    player1.move(5, 0, walls, hearts, enemies);
                    player1.tankTexture = IMG_LoadTexture(renderer, "player_right.png");
                }
                if (keystate[SDL_SCANCODE_LCTRL]) {
                    player1.shoot();
                }

                // Điều khiển Player 2
                if (keystate[SDL_SCANCODE_UP]) {
                    player2.move(0, -5, walls, hearts, enemies);
                    player2.tankTexture = IMG_LoadTexture(renderer, "player_up.png");
                }
                if (keystate[SDL_SCANCODE_DOWN]) {
                    player2.move(0, 5, walls, hearts, enemies);
                    player2.tankTexture = IMG_LoadTexture(renderer, "player_down.png");
                }
                if (keystate[SDL_SCANCODE_LEFT]) {
                    player2.move(-5, 0, walls, hearts, enemies);
                    player2.tankTexture = IMG_LoadTexture(renderer, "player_left.png");
                }
                if (keystate[SDL_SCANCODE_RIGHT]) {
                    player2.move(5, 0, walls, hearts, enemies);
                    player2.tankTexture = IMG_LoadTexture(renderer, "player_right.png");
                }
                if (keystate[SDL_SCANCODE_SPACE]) { // Player 2 bắn đạn bằng phím Ctrl trái
                    player2.shoot();
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

            if (state == MENU) {
                SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); //chọn màu
                SDL_RenderClear(renderer); // 👈 vẽ menu
            }

            else{
                player1.render(renderer);
                player2.render(renderer);


                for (int i=0; i < heartNumber; i++){
                    hearts[i].render(renderer);
                }

                for(int i=0; i<walls.size(); i++){
                    walls[i].render(renderer);
                }



                for (auto& enemy : enemies) {
                    enemy.render(renderer);
                }
            }
            SDL_RenderPresent(renderer); // hiển thị tất cả, phải có
        }


Game::~Game(){
            SDL_DestroyTexture(backgroundTexture);
            IMG_Quit();
            SDL_DestroyRenderer(renderer);
            SDL_DestroyWindow(window);
            SDL_Quit();
        }



void Game::update() {
    player1.updateBullets();
    player2.updateBullets();

    for (auto& enemy : enemies) { // cap nhat dan của dich
        enemy.move(walls, renderer);
        enemy.updateBullets();
        if (rand() % 100 < 2) {
            enemy.shoot();
        }
    }

    for (auto& enemy : enemies) { // dan dich ban tuong
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

    for (auto& bullet : player1.bullets) { // dan nguoi choi 1 ban tuong
        for (auto& wall : walls) {
            if (wall.active && SDL_HasIntersection(&bullet.rect, &wall.rect)) {
                wall.active = false;
                bullet.active = false;
                break;
            }
        }
    }

    for (auto& bullet : player2.bullets) { // dan nguoi choi 1 ban tuong
        for (auto& wall : walls) {
            if (wall.active && SDL_HasIntersection(&bullet.rect, &wall.rect)) {
                wall.active = false;
                bullet.active = false;
                break;
            }
        }
    }
    for (auto& bullet : player2.bullets) { // dan nguoi choi 2 ban tuong
        for (auto& wall : walls) {
            if (wall.active && SDL_HasIntersection(&bullet.rect, &wall.rect)) {
                wall.active = false;
                bullet.active = false;
                break;
            }
        }
    }

    for (auto& bullet : player1.bullets) { // dan nguoi choi 1 ban dich
        for (auto& enemy : enemies) {
            if (enemy.active && SDL_HasIntersection(&bullet.rect, &enemy.rect)) {
                enemy.active = false;
                bullet.active = false;
            }
        }
    }
        for (auto& bullet : player2.bullets) { // dan nguoi choi 2 ban dich
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
    handleEvents();
}

for (auto& enemy : enemies) { // dan dich ban nguoi choi 1, 2
    for (auto& bullet : enemy.bullets) {
        // Update
            if (SDL_HasIntersection(&bullet.rect, &player1.rect)) {
                bullet.active = false;
                player1.RemainingLives -= 1;
            }
            if (SDL_HasIntersection(&bullet.rect, &player2.rect)) {
                bullet.active = false;
                player2.RemainingLives -= 1;
            }
            if(player1.RemainingLives == 0 || player2.RemainingLives == 0){
                state = GAME_OVER;
                reset();
                return;
            }
        }
}

    for (auto& bullet : player1.bullets) { // dan nguoi choi 1 ban nguoi choi 2
        if (SDL_HasIntersection(&bullet.rect, &player2.rect)) {
            state = GAME_OVER;
            reset();
        }
    }

    for (auto& bullet : player2.bullets) { // dan nguoi choi 2 ban nguoi choi 1
        if (SDL_HasIntersection(&bullet.rect, &player1.rect)) {
            state = GAME_OVER;
            reset();
        }
    }
}

/*void Game::generateWalls(){
    /*for (int i = 3; i < MAP_HEIGHT - 3; i += 2) {
        for (int j = 3; j < MAP_WIDTH - 3; j += 2) {
            Wall w = Wall(j * TILE_SIZE, i * TILE_SIZE);
            walls.push_back(w);
        }
    }*/
    /*Wall w = Wall(2 * TILE_SIZE, 3 * TILE_SIZE);
    walls.push_back(w);*/

    /*walls.clear();
    for (int i = 0; i < 30; ++i) {
        int wx, wy;
        bool validPosition = false;
        while (!validPosition) {
            wx = (rand() % (MAP_WIDTH - 2) + 1) * TILE_SIZE;
            wy = (rand() % (MAP_HEIGHT - 2) + 1) * TILE_SIZE;
            validPosition = true;


            if (wx == player1.x|| wx == player2.x) {
                validPosition = false;
                //break;
            }


            for (const auto& enemy : enemies) {
                if (enemy.active && enemy.x == wx && enemy.y == wy) {
                    validPosition = false;
                    //break;
                }
            }
            //break;

            for (const auto& heart : hearts) {  // Di chuyển vào vòng while
                if (heart.active && heart.x == wx && heart.y == wy) {
                    validPosition = false;
                    //break;
                }
            }
            //break;
        }
        if(validPosition) walls.push_back(Wall(wx, wy, renderer));
    }
}*/

void Game::spawnEnemies() {
    enemies.clear();
    for (int i = 0; i < enemyNumber; ++i) {
        int ex, ey;
        bool validPosition = false;
        while (!validPosition) {
            ex = (rand() % (MAP_WIDTH - 2) + 1) * TILE_SIZE;
            ey = (rand() % (MAP_HEIGHT - 2) + 1) * TILE_SIZE;
            validPosition = true;
            if((player1.x == ex && player1.y == ey) || (player2.x == ex && player2.y == ey)){
                validPosition = false;
            }
            for (const auto& wall : walls) {
                if (wall.active && wall.x == ex && wall.y == ey) {
                    validPosition = false;
                    //break;
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
            if ((player1.x == hx && player1.y == hy) || (player2.x == hx && player2.y == hy)){
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

