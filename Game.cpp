#include "Game.h"
using namespace std;

Game::Game(){
            running = true;
            state = MENU;
            if(SDL_Init(SDL_INIT_EVERYTHING) < 0){
                cout << "SDL could not initialize! SDL_Error: " << SDL_GetError() << endl;
                running = false;
            }

            if (SDL_Init(SDL_INIT_AUDIO) < 0 || Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
                cout << "SDL_mixer could not initialize! Error: " << Mix_GetError() << endl;
                running = false;
                return;
            }

            if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
                cout << "SDL_image không thể khởi tạo! IMG_Error: " << IMG_GetError() << std::endl;
                running = false;
            }
            if (TTF_Init() == -1) {
                cout << "TTF init failed: " << TTF_GetError() << endl;
            }

            font = TTF_OpenFont("tank_font.ttf", 24);
            if (!font) {
                cout << "Failed to load font: " << TTF_GetError() << std::endl;
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
            backgroundMusic = Mix_LoadMUS("Soundtrack.mp3");
               if (!backgroundMusic) {
                    cout << "Failed to load music! Error: " << Mix_GetError() << endl;
                    running = false;
                } else {
                    Mix_PlayMusic(backgroundMusic, 1);
                }
                Mix_VolumeMusic(10);
            menuTexture = IMG_LoadTexture(renderer, "menu.png");
            explosionTexture = IMG_LoadTexture(renderer, "explosion.png");
            shootSound = Mix_LoadWAV("fireSound.wav");
            if (shootSound == nullptr) {
                cout << "Failed to load shoot sound! SDL_mixer Error: " << Mix_GetError() << endl;
            }
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
                if(state == PLAYING){
                    update();
                    render();
                    SDL_Delay(16);
                }
                else if(state == MENU){
                    renderMenu();
                }
            }
        }

void Game::initMode(GameMode mode){
    if(state != MENU && state != GAME_OVER && state != WIN) return;
    if(level > 9){
        if(scoreP1 > scoreP2){
            winnerTexture = IMG_LoadTexture(renderer, "P1win.png");
        }
        else winnerTexture = IMG_LoadTexture(renderer, "P2win.png");
        renderWinner();
        SDL_Delay(3000);
        state = MENU;
        level = 0;
        return;
    }

    enemies.clear();
    hearts.clear();
    gameMap.walls.clear();
    gameMap.waters.clear();
    gameMap.bushs.clear();
    gameMap.stones.clear();
    gameMap.ices.clear();

    player1 = PlayerTank(TILE_SIZE*10 - 1, MAP_HEIGHT * TILE_SIZE - 1, renderer);
    string filename;

    if(state == WIN){
        level++;
        filename = "gameMaps//" + to_string(level) + ".txt";
        if(mode == GameMode::PVP){
            player2 = PlayerTank(TILE_SIZE*15, MAP_HEIGHT * TILE_SIZE - 1, renderer);
            spawnHearts();
            gameMap.loadFromFile(filename, renderer);
            walls = gameMap.walls;
            waters = gameMap.waters;
            bushs = gameMap.bushs;
            stones = gameMap.stones;
            ices = gameMap.ices;
            spawnEnemies();
        }
        else{
            gameMap.loadFromFile(filename, renderer);
            walls = gameMap.walls;
            waters = gameMap.waters;
            bushs = gameMap.bushs;
            stones = gameMap.stones;
            ices = gameMap.ices;
            spawnHearts();
            spawnEnemies();
        }
    }

    else if(state == GAME_OVER){
        filename = "gameMaps//" + to_string(level) + ".txt";
        if(mode == GameMode::PVP){
            player2 = PlayerTank(TILE_SIZE*15, MAP_HEIGHT * TILE_SIZE - 1, renderer);
            spawnHearts();
            gameMap.loadFromFile(filename, renderer);
            walls = gameMap.walls;
            waters = gameMap.waters;
            bushs = gameMap.bushs;
            stones = gameMap.stones;
            ices = gameMap.ices;
            spawnEnemies();
        }
        else{
            gameMap.loadFromFile(filename, renderer);
            walls = gameMap.walls;
            waters = gameMap.waters;
            bushs = gameMap.bushs;
            stones = gameMap.stones;
            ices = gameMap.ices;
            spawnHearts();
            spawnEnemies();
        }
    }

    else{
        filename = "gameMaps//" + to_string(level) + ".txt";
        if(mode == GameMode::PVP){
            player2 = PlayerTank(TILE_SIZE*15, MAP_HEIGHT * TILE_SIZE - 1, renderer);
            spawnHearts();
            gameMap.loadFromFile(filename, renderer);
            walls = gameMap.walls;
            waters = gameMap.waters;
            bushs = gameMap.bushs;
            stones = gameMap.stones;
            ices = gameMap.ices;
            spawnEnemies();
        }
        else{
            gameMap.loadFromFile(filename, renderer);
            walls = gameMap.walls;
            waters = gameMap.waters;
            bushs = gameMap.bushs;
            stones = gameMap.stones;
            ices = gameMap.ices;
            spawnHearts();
            spawnEnemies();
        }
    }

    state = PLAYING;
}

/*void Game::reset() {
    running = true;

    enemies.clear();
    hearts.clear();
    walls.clear();
    waters.clear();
    bushs.clear();
    stones.clear();
    ices.clear();

    initMode(mode);
    state = PLAYING;
}*/


void Game::handleEvents() {
        SDL_Event event;
        const Uint8* keystate = SDL_GetKeyboardState(NULL); // Lấy trạng thái bàn phím

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
                break;
            }
            /*else if (event.type == SDL_KEYDOWN){
                if(event.key.keysym.sym == SDLK_ESCAPE){
                    running = false;
                    break;
                }
            }*/
            else if (state == MENU) {
                if (event.type == SDL_KEYDOWN) {
                    switch (event.key.keysym.sym) {
                        case SDLK_1:
                            mode = GameMode::PVE;
                            level = 0;
                            scoreP1 = 0;
                            scoreP2 = 0;
                            initMode(mode);
                            state = PLAYING;
                            break;
                        case SDLK_2:
                            mode = GameMode::PVP;
                            level = 0;
                            scoreP1 = 0;
                            scoreP2 = 0;
                            initMode(mode);
                            state = PLAYING;
                            break;
                        case SDLK_RETURN:
                            state = PLAYING;
                        default:
                            return;
                    }
                }  // 👈 xử lý menu ở đây
            }
            else if (state == PLAYING && event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_RETURN) {
                state = MENU;
            }
            else if(event.type == SDL_KEYDOWN){
                // Điều khiển Player 1 (Phím WASD)
                if (keystate[SDL_SCANCODE_W]) {
                    player1.move(0, -5, walls, hearts, enemies, stones, bushs, waters);
                    player1.tankTexture = IMG_LoadTexture(renderer, "player_up.png");
                }
                if (keystate[SDL_SCANCODE_S]) {
                    player1.move(0, 5, walls, hearts, enemies, stones, bushs, waters);
                    player1.tankTexture = IMG_LoadTexture(renderer, "player_down.png");
                }
                if (keystate[SDL_SCANCODE_A]) {
                    player1.move(-5, 0, walls, hearts, enemies, stones, bushs, waters);
                    player1.tankTexture = IMG_LoadTexture(renderer, "player_left.png");
                }
                if (keystate[SDL_SCANCODE_D]) {
                    player1.move(5, 0, walls, hearts, enemies, stones, bushs, waters);
                    player1.tankTexture = IMG_LoadTexture(renderer, "player_right.png");
                }
                if (keystate[SDL_SCANCODE_LCTRL]) {
                    player1.shoot(renderer);
                    Mix_PlayChannel(-1, shootSound, 0);
                }

                // Điều khiển Player 2 nếu có
                if(mode == GameMode::PVP){
                    if (keystate[SDL_SCANCODE_UP]) {
                        player2.move(0, -5, walls, hearts, enemies, stones, bushs, waters);
                        player2.tankTexture = IMG_LoadTexture(renderer, "player_up.png");
                    }
                    if (keystate[SDL_SCANCODE_DOWN]) {
                        player2.move(0, 5, walls, hearts, enemies, stones, bushs, waters);
                        player2.tankTexture = IMG_LoadTexture(renderer, "player_down.png");
                    }
                    if (keystate[SDL_SCANCODE_LEFT]) {
                        player2.move(-5, 0, walls, hearts, enemies, stones, bushs, waters);
                        player2.tankTexture = IMG_LoadTexture(renderer, "player_left.png");
                    }
                    if (keystate[SDL_SCANCODE_RIGHT]) {
                        player2.move(5, 0, walls, hearts, enemies, stones, bushs, waters);
                        player2.tankTexture = IMG_LoadTexture(renderer, "player_right.png");
                    }
                    if (keystate[SDL_SCANCODE_SPACE]) { // Player 2 bắn đạn bằng phím Ctrl trái
                        player2.shoot(renderer);
                        Mix_PlayChannel(-1, shootSound, 0);
                    }
                }
            }
        }
    }
void Game::render(){
            SDL_SetRenderDrawColor(renderer, 192, 192, 192, 255); //chọn màu
            SDL_RenderClear(renderer); // tô toàn bộ màu vừa chọn

            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // chọn màu mới
            for(int i=1; i <= MAP_HEIGHT; i++){
                for(int j=1; j <= MAP_HEIGHT; j++){
                    SDL_Rect tile = {j*TILE_SIZE, i*TILE_SIZE, TILE_SIZE, TILE_SIZE}; // tạo hcn tên tile (x,y,cr,cd)
                    SDL_RenderFillRect(renderer, &tile);
                }
            }

            renderLevel();
            renderRemainingLive(mode);


                player1.render(renderer);

                if(mode == GameMode::PVP){
                    renderScore();
                    player2.render(renderer);
                }


                for (int i=0; i < heartNumber; i++){
                    hearts[i].render(renderer);
                }

                for(int i=0; i<walls.size(); i++){
                    walls[i].render(renderer);
                }

                for(int i=0; i<waters.size(); i++){
                    waters[i].render(renderer);
                }

                for(int i=0; i<bushs.size(); i++){
                    bushs[i].render(renderer);
                }
                for(int i=0; i<stones.size(); i++){
                    stones[i].render(renderer);
                }
                for(int i=0; i<ices.size(); i++){
                    ices[i].render(renderer);
                }

                for (auto& enemy : enemies) {
                    enemy.render(renderer);
                }
            SDL_RenderPresent(renderer); // hiển thị tất cả, phải có
    }

void Game::renderLevel(){
    SDL_Color white = {0, 0, 0};

    string p1 = "Level: " + to_string(level);

    SDL_Surface* textSurface1 = TTF_RenderText_Blended(font, p1.c_str(), white);
    SDL_Texture* textTexture1 = SDL_CreateTextureFromSurface(renderer, textSurface1);
    SDL_Rect dstRect1 = {TILE_SIZE * 28, TILE_SIZE * 3, textSurface1->w, textSurface1->h}; // tuỳ chỉnh vị trí


    SDL_RenderCopy(renderer, textTexture1, NULL, &dstRect1);

    // Clean up
    SDL_FreeSurface(textSurface1);
    SDL_DestroyTexture(textTexture1);
}
void Game::renderScore(){
    SDL_Color white = {0, 0, 0};

    string p1 = "Player 1: " + to_string(scoreP1);
    string p2 = "Player 2: " + to_string(scoreP2);

    SDL_Surface* textSurface1 = TTF_RenderText_Blended(font, p1.c_str(), white);
    SDL_Texture* textTexture1 = SDL_CreateTextureFromSurface(renderer, textSurface1);
    SDL_Rect dstRect1 = {TILE_SIZE * 28, TILE_SIZE * 10, textSurface1->w, textSurface1->h}; // tuỳ chỉnh vị trí

    SDL_Surface* textSurface2 = TTF_RenderText_Blended(font, p2.c_str(), white);
    SDL_Texture* textTexture2 = SDL_CreateTextureFromSurface(renderer, textSurface2);
    SDL_Rect dstRect2 = {TILE_SIZE * 28, TILE_SIZE * 11, textSurface2->w, textSurface2->h}; // tuỳ chỉnh vị trí

    SDL_RenderCopy(renderer, textTexture1, NULL, &dstRect1);
    SDL_RenderCopy(renderer, textTexture2, NULL, &dstRect2);

    // Clean up
    SDL_FreeSurface(textSurface1);
    SDL_DestroyTexture(textTexture1);
    SDL_FreeSurface(textSurface2);
    SDL_DestroyTexture(textTexture2);
}



void Game::renderMenu(){
    //SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); //chọn màu
    SDL_RenderClear(renderer);// 👈 vẽ menu
    SDL_RenderCopy(renderer, menuTexture, nullptr, nullptr); // Hiển thị ảnh nền
    SDL_RenderPresent(renderer);
}

void Game::renderWinner(){
    SDL_RenderClear(renderer);// 👈 vẽ menu
    SDL_RenderCopy(renderer, winnerTexture, nullptr, nullptr); // Hiển thị ảnh nền
    SDL_RenderPresent(renderer);
}

void Game::renderRemainingLive(GameMode mode){
    SDL_Color white = {0, 0, 0};

    string p1 = "LiveP1: " + to_string(player1.RemainingLives);
    string p2 = "LiveP2: " + to_string(player2.RemainingLives);

    SDL_Surface* textSurface1 = TTF_RenderText_Blended(font, p1.c_str(), white);
    SDL_Texture* textTexture1 = SDL_CreateTextureFromSurface(renderer, textSurface1);
    SDL_Rect dstRect1 = {TILE_SIZE * 1, TILE_SIZE * 28, textSurface1->w, textSurface1->h}; // tuỳ chỉnh vị trí

    SDL_Surface* textSurface2 = TTF_RenderText_Blended(font, p2.c_str(), white);
    SDL_Texture* textTexture2 = SDL_CreateTextureFromSurface(renderer, textSurface2);
    SDL_Rect dstRect2 = {TILE_SIZE * 1, TILE_SIZE * 29, textSurface2->w, textSurface2->h}; // tuỳ chỉnh vị trí

    SDL_RenderCopy(renderer, textTexture1, NULL, &dstRect1);
    if(mode == GameMode::PVP) SDL_RenderCopy(renderer, textTexture2, NULL, &dstRect2);

    // Clean up
    SDL_FreeSurface(textSurface1);
    SDL_DestroyTexture(textTexture1);
    SDL_FreeSurface(textSurface2);
    SDL_DestroyTexture(textTexture2);
}


Game::~Game(){
            SDL_DestroyTexture(menuTexture);
            IMG_Quit();
            SDL_DestroyRenderer(renderer);
            SDL_DestroyWindow(window);
            TTF_CloseFont(font);
            TTF_Quit();
            SDL_Quit();
        }



void Game::update() {
    player1.updateBullets();
    for (auto& enemy : enemies) { // cap nhat dan của dich
        enemy.move(walls, renderer,stones, bushs, waters);
        enemy.updateBullets();
        if (rand() % 100 < 2) {
            enemy.shoot(renderer);
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
    for (auto& bullet : player1.bullets) { // dan nguoi choi 1 ban da
        for (auto& stone : stones) {
            if (SDL_HasIntersection(&bullet.rect, &stone.rect)) {
                bullet.active = false;
                break;
            }
        }
    }



    for (auto& bullet : player1.bullets) { // dan nguoi choi 1 ban dich
        for (auto& enemy : enemies) {
            if (enemy.active && SDL_HasIntersection(&bullet.rect, &enemy.rect)) {
                SDL_RenderCopy(renderer, explosionTexture, nullptr, &enemy.rect);
                enemy.active = false;
                bullet.active = false;
            }
        }
    }
    for (auto& enemy : enemies) { // dan dich ban nguoi choi 1
        for (auto& bullet : enemy.bullets) {
            // Update
                if (SDL_HasIntersection(&bullet.rect, &player1.rect)) {
                    bullet.active = false;
                    player1.RemainingLives -= 1;
                }
                if(player1.RemainingLives == 0){
                    scoreP1 = 0;
                    state = GAME_OVER;
                    initMode(mode);
                    return;
                }
            }
        }

    if(mode == GameMode::PVE){
        if (enemies.empty()) {
            state = WIN;
            scoreP1++;
            initMode(mode);
        }
    }
    else{
        player2.updateBullets();

        for (auto& bullet : player2.bullets) { // dan nguoi choi 2 ban tuong
            for (auto& wall : walls) {
                if (wall.active && SDL_HasIntersection(&bullet.rect, &wall.rect)) {
                    wall.active = false;
                    bullet.active = false;
                    break;
                }
            }
        }
        for (auto& bullet : player2.bullets) { // dan nguoi choi 1 ban tuong
            for (auto& stone : stones) {
                if (SDL_HasIntersection(&bullet.rect, &stone.rect)) {
                    bullet.active = false;
                    break;
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
                    if(player1.RemainingLives == 0){
                        scoreP2++;
                        state = GAME_OVER;
                        initMode(mode);
                        return;
                    }
                    else if(player2.RemainingLives == 0){
                        scoreP1++;
                        state = GAME_OVER;
                        initMode(mode);
                        return;
                    }
                }
            }

        for (auto& bullet : player1.bullets) { // dan nguoi choi 1 ban nguoi choi 2
            if (SDL_HasIntersection(&bullet.rect, &player2.rect)) {
                state = WIN;
                scoreP1 += 1;
                initMode(mode);
            }
        }

        for (auto& bullet : player2.bullets) { // dan nguoi choi 2 ban nguoi choi 1
            if (SDL_HasIntersection(&bullet.rect, &player1.rect)) {
                state = WIN;
                scoreP2 += 1;
                initMode(mode);
            }
        }
    }

    enemies.erase(std::remove_if(enemies.begin(), enemies.end(),
    [](EnemyTank& e) { return !e.active; }), enemies.end());
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
            if( (player1.x == ex && player1.y == ey) ||(mode == GameMode::PVP && (player2.x == ex && player2.y == ey))){
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
            if ((player1.x == hx && player1.y == hy) || (mode == GameMode::PVP &&(player2.x == hx && player2.y == hy))){
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

