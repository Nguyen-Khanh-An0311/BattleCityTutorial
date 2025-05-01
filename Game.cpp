#include "Game.h"
using namespace std;

Game::Game(){
            running = true;
            state = MENU;
            mode = GameMode::NONE;

            if(SDL_Init(SDL_INIT_EVERYTHING) < 0){
                cout << "SDL could not initialize! SDL_Error: " << SDL_GetError() << endl;
                running = false;
            }

            if (SDL_Init(SDL_INIT_AUDIO) < 0 || Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
                cout << "SDL_mixer could not initialize! Error: " << Mix_GetError() << endl;
                running = false;
                return;
            }
            Mix_AllocateChannels(32);
            AudioManager::Init();


            if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
                cout << "SDL_image không thể khởi tạo! IMG_Error: " << IMG_GetError() << std::endl;
                running = false;
            }
            if (TTF_Init() == -1) {
                cout << "TTF init failed: " << TTF_GetError() << endl;
            }

            font = TTF_OpenFont("Font//prstartk.ttf", 30);
            fontLarge = TTF_OpenFont("Font//prstartk.ttf", 35);
            fontScore = TTF_OpenFont("Font//prstartk.ttf", 80);
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

            base = Base((MAP_WIDTH / 2)*TILE_SIZE, (MAP_HEIGHT - 1)*TILE_SIZE, renderer);
            gateOut = Gate(renderer);
            currentBoss = NULL;

            menuTexture = IMG_LoadTexture(renderer, "Image//logo.jpg");
            levelTexture = IMG_LoadTexture(renderer, "Image//level_flag.png");
            enemyTexture = IMG_LoadTexture(renderer, "Image//enemy.png");
            RML1 = IMG_LoadTexture(renderer, "Image//heart.png");
            RML2 = IMG_LoadTexture(renderer, "Image//heart.png");
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
                updateSoundState();
                switch(state){
                    case PAUSE:
                        renderPauseMenu();
                        break;
                    case MENU:
                        showMenu();
                        break;
                    case PLAYING:
                        handleEvents();
                        update();
                        render();
                        SDL_Delay(16);
                        break;
                    case SHOW_WINNER:
                        renderWinner();
                        state = MENU;
                        break;
            }
        }
}

void Game::handleEvents() {
        /*SDL_Event event;
        const Uint8* keystate = SDL_GetKeyboardState(NULL); // Lấy trạng thái bàn phím
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
                break;
            }
            else if (state == PLAYING && event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) {
                state = MENU;
            }
            else if(event.type == SDL_KEYDOWN){
                // Điều khiển Player 1 (Phím WASD)
                if (keystate[SDL_SCANCODE_W]) {
                    player1.move(0, -5, walls, hearts, enemies, stones, bushs, waters);
                }
                else if (keystate[SDL_SCANCODE_S]) {
                    player1.move(0, 5, walls, hearts, enemies, stones, bushs, waters);
                }
                else if (keystate[SDL_SCANCODE_A]) {
                    player1.move(-5, 0, walls, hearts, enemies, stones, bushs, waters);
                }
                else if (keystate[SDL_SCANCODE_D]) {
                    player1.move(5, 0, walls, hearts, enemies, stones, bushs, waters);
                }
                else if (keystate[SDL_SCANCODE_LCTRL]) {
                    player1.shoot(renderer);
                    Mix_PlayChannel(-1, shootSound, 0);
                }

                // Điều khiển Player 2 nếu có
                if(mode == GameMode::PVP){
                    if (keystate[SDL_SCANCODE_UP]) {
                        player2.move(0, -5, walls, hearts, enemies, stones, bushs, waters);
                    }
                    else if (keystate[SDL_SCANCODE_DOWN]) {
                        player2.move(0, 5, walls, hearts, enemies, stones, bushs, waters);
                    }
                    else if (keystate[SDL_SCANCODE_LEFT]) {
                        player2.move(-5, 0, walls, hearts, enemies, stones, bushs, waters);
                    }
                    else if (keystate[SDL_SCANCODE_RIGHT]) {
                        player2.move(5, 0, walls, hearts, enemies, stones, bushs, waters);
                    }
                    else if (keystate[SDL_SCANCODE_SPACE]) { // Player 2 bắn đạn bằng phím Ctrl trái
                        player2.shoot(renderer);
                        Mix_PlayChannel(-1, shootSound, 0);
                    }
                }
            }
        }*/
    SDL_Event event;
    while (SDL_PollEvent(&event)) { //bắt trạng thái phím đg chờ
        if (event.type == SDL_QUIT) {
            running = false;
        } else if (event.type == SDL_KEYDOWN) {
            switch (event.key.keysym.sym) {
                case SDLK_LCTRL:
                    player1.shoot(renderer);
                    /*Mix_PlayChannel(-1, shootSound, 0);
                    if (Mix_PlayChannel(-1, shootSound, 0) == -1) {
                        cerr << "Failed to play sound: " << Mix_GetError() << endl;
                    }*/
                    AudioManager::PlaySound(3, "shoot", 0);
                    break;
                case SDLK_SPACE:
                    if(mode == PVP){
                        player2.shoot(renderer);
                        //Mix_PlayChannel(-1, shootSound, 0);
                        AudioManager::PlaySound(3, "shoot", 0);
                    }
                    break;

                case SDLK_ESCAPE:
                    state = PAUSE;
                    break;
            }
        }
    }

    // Xử lý giữ phím để di chuyển mượt hơn
    const Uint8* keystates = SDL_GetKeyboardState(NULL); //liên tục lấy trạng thai phím

    // Player 1
                if (keystates[SDL_SCANCODE_W]) {
                    player1.move(0, -5, walls, hearts, enemies, stones, bushs, waters);
                }
                else if (keystates[SDL_SCANCODE_S]) {
                    player1.move(0, 5, walls, hearts, enemies, stones, bushs, waters);
                }
                else if (keystates[SDL_SCANCODE_A]) {
                    player1.move(-5, 0, walls, hearts, enemies, stones, bushs, waters);
                }
                else if (keystates[SDL_SCANCODE_D]) {
                    player1.move(5, 0, walls, hearts, enemies, stones, bushs, waters);
                }

    // Player 2
                if(mode == PVP){
                    if (keystates[SDL_SCANCODE_UP]) {
                        player2.move(0, -5, walls, hearts, enemies, stones, bushs, waters);
                    }
                    else if (keystates[SDL_SCANCODE_DOWN]) {
                        player2.move(0, 5, walls, hearts, enemies, stones, bushs, waters);
                    }
                    else if (keystates[SDL_SCANCODE_LEFT]) {
                        player2.move(-5, 0, walls, hearts, enemies, stones, bushs, waters);
                    }
                    else if (keystates[SDL_SCANCODE_RIGHT]) {
                        player2.move(5, 0, walls, hearts, enemies, stones, bushs, waters);
                    }
                }
}
void Game::update() {
    if(currentBoss){
        enemies.insert(enemies.begin(), currentBoss->enemiesFromHole.begin(), currentBoss->enemiesFromHole.end());
        currentBoss->enemiesFromHole.clear();
    }
    if(enemies.empty()){
        if(!currentBoss || currentBoss && !currentBoss->active){
            gateOut.spawn((MAP_WIDTH/2)*TILE_SIZE, (MAP_HEIGHT/2)*TILE_SIZE);
        }
    }
    player1.updateBullets();
    if (player1.cooldown > 0){
        player1.cooldown--;
        //cout << player1.cooldown << endl;
    }

        if (gateOut.active) {
            SDL_Rect gateRect = gateOut.getRect();
                if(mode == PVE){
                    if (SDL_HasIntersection(&player1.rect, &gateRect)) {
                        gateOut.active = false;
                        state = WIN;
                        initMode(mode); // gọi lại để load map mới
                    }
                }
                else {
                    if (SDL_HasIntersection(&player1.rect, &gateRect) && SDL_HasIntersection(&player2.rect, &gateRect)) {
                        gateOut.active = false;
                        state = WIN;
                        initMode(mode); // gọi lại để load map mới
                    }
                }
            }
    if(currentBoss && currentBoss->active) currentBoss->update();

    for (auto& enemy : enemies) { // cap nhat dan của dich
        enemy.move(walls, renderer,stones, bushs, waters, base);
        enemy.updateBullets();
        if (rand() % 100 < 2) {
            enemy.shoot(renderer);
        }
    }
    for (auto& enemy : enemies) { // dan dich ban tuong
        for (auto& bullet : enemy.bullets) {
            for (auto& wall : walls) {
                if (wall.active && SDL_HasIntersection(&bullet.rect, &wall.rect)) {
                    explosions.emplace_back(renderer, wall.x, wall.y);
                    wall.active = false;
                    bullet.active = false;
                    break;
                }
            }
        }
    }
    for (auto& enemy : enemies) { // dan dich ban tuong
        for (auto& bullet : enemy.bullets) {
            for (auto& stone : stones) {
                if (SDL_HasIntersection(&bullet.rect, &stone.rect)) {
                    explosions.emplace_back(renderer, bullet.x, bullet.y);
                    bullet.active = false;
                    break;
                }
            }
        }
    }


    for (auto& enemy : enemies) { // dan dich ban tru
        for (auto& bullet : enemy.bullets) {
            if (base.active && SDL_HasIntersection(&bullet.rect, &base.rect)) {
                explosions.emplace_back(renderer, base.x, base.y);
                bullet.active = false;
                base.active = false;
                state = SHOW_WINNER;
                break;
            }
        }
    }

    for (auto& bullet : player1.bullets) { // dan nguoi choi 1 ban tuong
        for (auto& wall : walls) {
            if (wall.active && SDL_HasIntersection(&bullet.rect, &wall.rect)) {
                explosions.emplace_back(renderer, wall.x, wall.y); //emplace_back: khởi tạo cùng lúc push_back
                wall.active = false;
                bullet.active = false;
                break;
            }
        }
    }
    for (auto& bullet : player1.bullets) { // dan nguoi choi 1 ban da
        for (auto& stone : stones) {
            if (SDL_HasIntersection(&bullet.rect, &stone.rect)) {
                explosions.emplace_back(renderer, bullet.x, bullet.y);
                bullet.active = false;
                break;
            }
        }
    }
    for (auto& bullet : player1.bullets) { // dan nguoi choi 1 ban nuoc
        for (auto& water : waters) {
            if (SDL_HasIntersection(&bullet.rect, &water.rect)) {
                //explosions.emplace_back(renderer, bullet.x, bullet.y);
                bullet.active = false;
                break;
            }
        }
    }

    for (auto& bullet : player1.bullets) { // dan nguoi choi 1 ban dich
        for (auto& enemy : enemies) {
            if (enemy.active && SDL_HasIntersection(&bullet.rect, &enemy.rect)) {
                player1.score++;
                explosions.emplace_back(renderer, enemy.x, enemy.y);
                enemy.active = false;
                bullet.active = false;
            }
        }
    }

    for (auto& enemy : enemies) { // dan dich ban nguoi choi 1
        for (auto& bullet : enemy.bullets) {
            // Update
                if (SDL_HasIntersection(&bullet.rect, &player1.rect)) {
                    explosions.emplace_back(renderer, player1.x, player1.y);
                    bullet.active = false;
                    player1.RemainingLives -= 1;
                }
                if(player1.RemainingLives == 0){
                    player1.active = false;
                }
            }
        }
    for (auto& bullet : player1.bullets) { // player 1 bắn boss
        if (currentBoss && currentBoss->active && SDL_HasIntersection(&currentBoss->destRect, &bullet.rect)) {
            explosions.emplace_back(renderer, bullet.x, bullet.y);
            currentBoss->RemainingLives -= 1;
            bullet.active = false;
            if(currentBoss->RemainingLives == 0){
                return;
            }
        }
    }

    if(currentBoss && currentBoss->active){ // dính chiêu của boss
        if(player1.cooldown == 0 && currentBoss->checkCollision(player1)){
            player1.RemainingLives -= 1;
            player1.cooldown = 120;
        }
        if(player1.RemainingLives == 0){
            player1.active = false;
        }
    }

    if(mode == GameMode::PVE){
        if (!player1.active) {
            state = SHOW_WINNER;
            return;
        }
    }
    else {
        player2.updateBullets();
        if(player2.cooldown > 0) player2.cooldown--;

        for (auto& bullet : player2.bullets) { // dan nguoi choi 2 ban tuong
            for (auto& wall : walls) {
                if (wall.active && SDL_HasIntersection(&bullet.rect, &wall.rect)) {
                    explosions.emplace_back(renderer, wall.x, wall.y);
                    wall.active = false;
                    bullet.active = false;
                    break;
                }
            }
        }
        for (auto& bullet : player2.bullets) { // dan nguoi choi 2 ban da
            for (auto& stone : stones) {
                if (SDL_HasIntersection(&bullet.rect, &stone.rect)) {
                    explosions.emplace_back(renderer, bullet.x, bullet.y);
                    bullet.active = false;
                    break;
                }
            }
        }
        for (auto& bullet : player2.bullets) { // dan nguoi choi 2 ban nuoc
            for (auto& water : waters) {
                if (SDL_HasIntersection(&bullet.rect, &water.rect)) {
                    explosions.emplace_back(renderer, bullet.x, bullet.y);
                    bullet.active = false;
                    break;
                }
            }
        }
        for (auto& bullet : player2.bullets) { // dan nguoi choi 2 ban dich
            for (auto& enemy : enemies) {
                if (enemy.active && SDL_HasIntersection(&bullet.rect, &enemy.rect)) {
                    player2.score++;
                    //explosions.emplace_back(renderer, enemy.x, enemy.y);
                    enemy.active = false;
                    bullet.active = false;
                }
            }
        }

        for (auto& enemy : enemies) { // dan dich ban nguoi choi 2
            for (auto& bullet : enemy.bullets) {
                // Update
                    if (SDL_HasIntersection(&bullet.rect, &player2.rect)) {
                        explosions.emplace_back(renderer, player2.x, player2.y);
                        bullet.active = false;
                        player2.RemainingLives -= 1;
                    }
                    else if(player2.RemainingLives == 0){
                        player2.active = false;
                        return;
                    }
                }
            }
        if(currentBoss && currentBoss->active){ // p2 dinh lua
            if(player2.cooldown == 0 && currentBoss->checkCollision(player2)){
                player2.RemainingLives -= 1;
                player2.cooldown = 120;
            }
            if(player2.RemainingLives == 0){
                player1.active = false;
            }
        }
        for (auto& bullet : player2.bullets) { // player 2 bắn boss
            if (currentBoss && currentBoss->active && SDL_HasIntersection(&currentBoss->destRect, &bullet.rect)) {
                explosions.emplace_back(renderer, bullet.x, bullet.y);
                currentBoss->RemainingLives -= 1;
                bullet.active = false;
                if(currentBoss->RemainingLives == 0){
                    return;
                }
            }
        }

        //xet thua cuoc
        if(!player1.active && !player2.active){
            state = SHOW_WINNER;
            return;
        }
    }

    enemies.erase(std::remove_if(enemies.begin(), enemies.end(),
    [](EnemyTank& e) { return !e.active; }), enemies.end());
}
void Game::updateSoundState(){
        switch (state) {
            case MENU:
                Mix_HaltChannel(-1);
                /*if(!Mix_Playing(0)){
                    AudioManager::PlaySound(0, "background", -1);
                }*/
            case PAUSE:
                Mix_HaltChannel(-1);
                break;
            case PLAYING:
                if(currentBoss && currentBoss->active){
                    if(currentBoss->name == "FireBoss") {
                        if (!Mix_Playing(1)) {
                            AudioManager::PlaySound(1,"fireboss", -1); // chỉ phát nếu chưa chạy
                        }
                    }
                    else {
                        if (!Mix_Playing(2)) {
                            AudioManager::PlaySound(2,"iceboss", -1); // chỉ phát nếu chưa chạy
                        }
                    }
                }
                break;
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

            SDL_Rect flagRect;
            flagRect.x = TILE_SIZE * 28;  // hoặc tùy bạn
            flagRect.y = TILE_SIZE * 1;  // vị trí dưới chữ số "1"
            flagRect.w = TILE_SIZE * 8;
            flagRect.h = TILE_SIZE * 8;
            SDL_RenderCopy(renderer, levelTexture, nullptr, &flagRect);

            SDL_Rect rect1;
            SDL_Rect rect2;
            rect1 = {TILE_SIZE * 5, TILE_SIZE * 27.75, TILE_SIZE * 1.5, TILE_SIZE * 1.5};
            rect2 = {TILE_SIZE * 5, TILE_SIZE * 28.75, TILE_SIZE * 1.5, TILE_SIZE * 1.5};
            SDL_RenderCopy(renderer, RML1, nullptr, &rect1); // Hiển thị ảnh nền

            renderLevel();
            //renderHeart();
            renderRemainingLive(mode);
            renderScore();
            for (auto& enemy : enemies) {
                enemy.render(renderer);
            }

            base.render(renderer);
            player1.render(renderer);

                if(mode == GameMode::PVP){
                    renderScore();
                    SDL_RenderCopy(renderer, RML2, nullptr, &rect2);
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
                if (currentBoss) {
                    if (currentBoss->RemainingLives > 0) {
                        currentBoss->render(renderer); // render bình thường
                    } else { // chết thì render animation chết, tắt âm thanh
                        currentBoss->Die(renderer);
                    }
                }
                for (auto it = explosions.begin(); it != explosions.end();) {
                    if (it->isFinished()) {
                        it = explosions.erase(it);
                    } else {
                        if(it->soundPlay){
                            AudioManager::PlaySound(4, "explosion", 0);
                            it->soundPlay = false;
                        }
                        it->render(renderer);
                        ++it;
                    }
                }
                if(gateOut.active){
                    gateOut.render(renderer);
                }

            SDL_RenderPresent(renderer); // hiển thị tất cả, phải có
    }

void Game::renderLevel(){
    SDL_Color white = {0, 0, 0};

    string p1 = to_string(level);

    SDL_Surface* textSurface1 = TTF_RenderText_Blended(fontScore, p1.c_str(), white);
    SDL_Texture* textTexture1 = SDL_CreateTextureFromSurface(renderer, textSurface1);
    SDL_Rect dstRect1 = {TILE_SIZE * 31, TILE_SIZE * 2, textSurface1->w, textSurface1->h}; // tuỳ chỉnh vị trí


    SDL_RenderCopy(renderer, textTexture1, NULL, &dstRect1);

    // Clean up
    SDL_FreeSurface(textSurface1);
    SDL_DestroyTexture(textTexture1);
}
void Game::renderScore(){
    SDL_Rect rect1;
    SDL_Rect rect2;
    rect1 = {TILE_SIZE * 32.25, TILE_SIZE * 10, TILE_SIZE , TILE_SIZE };
    rect2 = {TILE_SIZE * 32.25, TILE_SIZE * 11, TILE_SIZE , TILE_SIZE };
    SDL_RenderCopy(renderer, enemyTexture, nullptr, &rect1); // Hiển thị ảnh nền
    if(mode == PVP) SDL_RenderCopy(renderer,enemyTexture , nullptr, &rect2);
    SDL_Color white = {0, 0, 0};

    string p1 = "P1: " + to_string(player1.score);
    string p2 = "P1: " + to_string(player2.score);

    SDL_Surface* textSurface1 = TTF_RenderText_Blended(font, p1.c_str(), white);
    SDL_Texture* textTexture1 = SDL_CreateTextureFromSurface(renderer, textSurface1);
    SDL_Rect dstRect1 = {TILE_SIZE * 28, TILE_SIZE * 10, textSurface1->w, textSurface1->h}; // tuỳ chỉnh vị trí

    SDL_Surface* textSurface2 = TTF_RenderText_Blended(font, p2.c_str(), white);
    SDL_Texture* textTexture2 = SDL_CreateTextureFromSurface(renderer, textSurface2);
    SDL_Rect dstRect2 = {TILE_SIZE * 28, TILE_SIZE * 11, textSurface2->w, textSurface2->h}; // tuỳ chỉnh vị trí*/

    SDL_RenderCopy(renderer, textTexture1, NULL, &dstRect1);
    if(mode == PVP) SDL_RenderCopy(renderer, textTexture2, NULL, &dstRect2);

    // Clean up
    SDL_FreeSurface(textSurface1);
    SDL_DestroyTexture(textTexture1);
    SDL_FreeSurface(textSurface2);
    SDL_DestroyTexture(textTexture2);
}
void Game::renderHeart(){
    SDL_Rect rect1;
    SDL_Rect rect2;
    rect1 = {TILE_SIZE * 5, TILE_SIZE * 27.75, TILE_SIZE * 1.5, TILE_SIZE * 1.5};
    rect2 = {TILE_SIZE * 5, TILE_SIZE * 28.75, TILE_SIZE * 1.5, TILE_SIZE * 1.5};
    RML1 = IMG_LoadTexture(renderer, "heart.png");
    RML2 = IMG_LoadTexture(renderer, "heart.png");
    SDL_RenderCopy(renderer, RML1, nullptr, &rect1); // Hiển thị ảnh nền
    if(mode == PVP) SDL_RenderCopy(renderer, RML2, nullptr, &rect2);
}
void Game::renderWinner(){
    //Mix_HaltChannel(-1);
    SDL_Color color = {255, 255, 0}; // Vàng rực rỡ
    SDL_Surface* textSurface;
    SDL_Surface* textMVP;
    if(base.active && !currentBoss->active && enemies.empty()){
        textSurface = TTF_RenderText_Blended(font, "YOU WIN!!!", color);
    }
    else {
        textSurface = TTF_RenderText_Blended(font, "GAME OVER!!!", color);
    }
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    /*if(mode == PVE){
        textMVP = TTF_RenderText_Blended(font, player1.score > player2.score ? "MVP: Player 1" : "MVP: Player 2", color);
    }*/
    int textW = textSurface->w;
    int textH = textSurface->h;
    SDL_FreeSurface(textSurface);

    int screenW = SCREEN_WIDTH;
    int screenH = SCREEN_HEIGHT;

    int startY = -textH; // Bắt đầu từ trên cùng
    int endY = (screenH - textH) / 2; // Vị trí giữa

    Uint32 start = SDL_GetTicks();
    Uint32 duration = 2000; // 2s

    SDL_Event e;

    while (SDL_GetTicks() - start < duration) {
        // Tính toán vị trí chữ theo thời gian
        float t = (float)(SDL_GetTicks() - start) / duration;
        int currentY = startY + (int)((endY - startY) * t);

        // Xử lý sự kiện
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT)
                exit(0);
        }

        SDL_RenderClear(renderer);

        SDL_Rect dstRect = {(screenW - textW) / 2, currentY, textW, textH};
        SDL_RenderCopy(renderer, textTexture, nullptr, &dstRect);

        SDL_RenderPresent(renderer);
        SDL_Delay(16); // ~60 FPS
    }

    // Hiển thị thêm vài giây nếu muốn
    SDL_Delay(2000);
}
void Game::renderRemainingLive(GameMode mode){
    SDL_Color white = {0, 0, 0};

    string p1 = "P1: " + to_string(player1.RemainingLives);
    string p2 = "P2: " + to_string(player2.RemainingLives);

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

void Game::showMenu() {
    if(state != MENU) return;
    level = 0;
    scoreP1 = 0;
    scoreP2 = 0;
    delete currentBoss;
    bool inMenu = true;
    int selectedOption = 0; // 0 = Start Game, 1 = Exit
    SDL_Event event;
    SDL_Color white = {255, 255, 255};
    SDL_Color yellow = {255, 255, 0};
    int highlightAlpha = 128;
    bool alphaIncreasing = true;
    Uint32 lastTime = SDL_GetTicks();
    int lineSpacing = 40;

    while (inMenu) {
        // Vẽ menu
        SDL_RenderClear(renderer);// 👈 vẽ menu
        SDL_RenderCopy(renderer, menuTexture, nullptr, nullptr); // Hiển thị ảnh nền
        //SDL_RenderPresent(renderer);

        Uint32 currentTime = SDL_GetTicks();
        if (currentTime > lastTime + 30) { // cập nhật mỗi ~30ms
            if (alphaIncreasing) {
                highlightAlpha += 8;
                if (highlightAlpha >= 192) {
                    highlightAlpha = 192;
                    alphaIncreasing = false;
                }
            } else {
                highlightAlpha -= 8;
                if (highlightAlpha <= 64) {
                    highlightAlpha = 64;
                    alphaIncreasing = true;
                }
            }
            lastTime = currentTime;
        }

        // T?o các chuỗi menu
        string options[3] = {"Start Game", "Instruction", "Exit"};
        int totalMenuHeight = 0;
        for (int i = 0; i < 3; ++i) {
            TTF_Font* currentFont = (i == selectedOption) ? fontLarge : font;
            int w, h;
            TTF_SizeText(currentFont, options[i].c_str(), &w, &h);
            totalMenuHeight += h + lineSpacing;
        }
        totalMenuHeight -= lineSpacing; // không tính khoảng sau dòng cuối
        int yOffset = (SCREEN_HEIGHT - totalMenuHeight) / 2 + 200;

        for (int i = 0; i < 3; ++i) {
            SDL_Color color = (i == selectedOption) ? yellow : white;
            TTF_Font* currentFont = (i == selectedOption) ? fontLarge : font;

            SDL_Surface* surface = TTF_RenderText_Blended(currentFont, options[i].c_str(), color);
            SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
            //SDL_Rect dstRect = { SCREEN_WIDTH / 2 - surface->w / 2, 200 + i * 50, surface->w, surface->h };
            SDL_Rect dstRect = {
                SCREEN_WIDTH / 2 - surface->w / 2,
                yOffset,
                surface->w,
                surface->h
            };

            // Highlight động cho lựa chọn đang được chọn
            if (i == selectedOption) {
                SDL_Rect highlightRect = {
                    dstRect.x - 10,
                    dstRect.y - 5,
                    dstRect.w + 20,
                    dstRect.h + 10
                };
                SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
                SDL_SetRenderDrawColor(renderer, 119, 139, 153, highlightAlpha);
                SDL_RenderFillRect(renderer, &highlightRect);
            }

            SDL_RenderCopy(renderer, texture, NULL, &dstRect);
            SDL_FreeSurface(surface);
            SDL_DestroyTexture(texture);

            yOffset += dstRect.h + lineSpacing;

        }

        SDL_RenderPresent(renderer);

        // X? lý s? ki?n bàn phím trong menu
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
                inMenu = false;
                return;
            } else if (event.type == SDL_KEYDOWN) {
                switch (event.key.keysym.sym) {
                    case SDLK_UP:
                        selectedOption = (selectedOption - 1 + 3) % 3;
                        break;
                    case SDLK_DOWN:
                        selectedOption = (selectedOption + 1) % 3;
                        break;
                    case SDLK_RETURN:
                    case SDLK_KP_ENTER:
                        if (selectedOption == 0) {
                            ChooseMode();
                            inMenu = false;
                        }
                        else if (selectedOption == 1){
                            renderInstruction();
                            inMenu = false;
                        }
                        else {
                            running = false;
                            return; // Thoát game
                        }
                        break;
                }
            }
        }

        SDL_Delay(100); // Làm ch?m vòng l?p m?t chút cho d? nhìn
    }
}
void Game::ChooseMode(){
    if(state != MENU) return;
    bool inChooseMode = true;
    int selectedMode = 0; //0 = 1P, 1= 2P
    SDL_Event event;
    SDL_Color white = {255, 255, 255};
    SDL_Color yellow = {255, 255, 0};
    int highlightAlpha = 128;
    bool alphaIncreasing = true;
    Uint32 lastTime = SDL_GetTicks();
    int lineSpacing = 40;
    while (inChooseMode) {
        // Vẽ nền menu
        SDL_RenderClear(renderer);// 👈 vẽ menu
        SDL_RenderCopy(renderer, menuTexture, nullptr, nullptr); // Hiển thị ảnh nền
        //SDL_RenderPresent(renderer);

        Uint32 currentTime = SDL_GetTicks();
        if (currentTime > lastTime + 30) { // cập nhật mỗi ~30ms
            if (alphaIncreasing) {
                highlightAlpha += 8;
                if (highlightAlpha >= 192) {
                    highlightAlpha = 192;
                    alphaIncreasing = false;
                }
            } else {
                highlightAlpha -= 8;
                if (highlightAlpha <= 64) {
                    highlightAlpha = 64;
                    alphaIncreasing = true;
                }
            }
            lastTime = currentTime;
        }

        // T?o các chu?i menu
        string options[3] = {"Player vs Emenies", "Player vs Player", "<-"};
        int totalMenuHeight = 0;
        for (int i = 0; i < 3; ++i) {
            TTF_Font* currentFont = (i == selectedMode) ? fontLarge : font;
            int w, h;
            TTF_SizeText(currentFont, options[i].c_str(), &w, &h);
            totalMenuHeight += h + lineSpacing;
        }
        totalMenuHeight -= lineSpacing; // không tính khoảng sau dòng cuối
        int yOffset = (SCREEN_HEIGHT - totalMenuHeight) / 2 + 200;

        for (int i = 0; i < 3; ++i) {
            SDL_Color color = (i == selectedMode) ? yellow : white;
            SDL_Surface* surface = TTF_RenderText_Blended(font, options[i].c_str(), color);
            SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
            SDL_Rect dstRect = {
                SCREEN_WIDTH / 2 - surface->w / 2,
                yOffset,
                surface->w,
                surface->h
            };

            if (i == selectedMode) {
                SDL_Rect highlightRect = {
                    dstRect.x - 10,
                    dstRect.y - 5,
                    dstRect.w + 20,
                    dstRect.h + 10
                };
                SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
                SDL_SetRenderDrawColor(renderer, 119, 139, 153, highlightAlpha);
                SDL_RenderFillRect(renderer, &highlightRect);
            }

            SDL_RenderCopy(renderer, texture, NULL, &dstRect);
            SDL_FreeSurface(surface);
            SDL_DestroyTexture(texture);
            yOffset += dstRect.h + lineSpacing;
        }

        SDL_RenderPresent(renderer);

        // X? lý s? ki?n bàn phím trong menu
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
                inChooseMode = false;
                return;
            } else if (event.type == SDL_KEYDOWN) {
                switch (event.key.keysym.sym) {
                    case SDLK_UP:
                        selectedMode = (selectedMode - 1 + 3) % 3;
                        break;
                    case SDLK_DOWN:
                        selectedMode = (selectedMode + 1) % 3;
                        break;
                    case SDLK_RETURN:
                    case SDLK_KP_ENTER:
                        if (selectedMode == 0) {
                            mode = PVE;
                            cout<<"Chon Mode 1 Nguoi Choi";
                            inChooseMode = false; // Ch?n 1P
                            initMode(mode);
                            state = PLAYING;

                        }
                        else if(selectedMode == 1){
                            mode = PVP;
                            cout<<"Chon Mode 2 Nguoi Choi";
                            inChooseMode = false;//Ch?n 2p
                            initMode(mode);
                            state = PLAYING;
                        }
                        else{
                            showMenu();
                            inChooseMode = false;
                            return;
                        }

                        break;
                }
            }
        }

        SDL_Delay(100); // Làm chậm vòng lăp một chút cho dễ nhìn
    }
}
void Game::renderInstruction(){
    if(state != MENU) return;
    bool renderInstruction = true;
    SDL_Event event;

    while (renderInstruction){
        SDL_RenderClear(renderer);// 👈 vẽ menu
        SDL_RenderCopy(renderer, menuTexture, nullptr, nullptr); // Hiển thị ảnh nền

        SDL_Color color = {255, 255, 255}; // Màu trắng

        const char* instructions[] = {
            "PLAYER 1: "
            " WASD: Move",
            " LEFT_CONTROL: Shoot",
            "PLAYER 2: ",
            " U,D,L,R: Move",
            " SPACE: Shoot",
            "*Note: If you wanna go back to MENU at any time, press ENTER*"
        };

        int y = 100;
        for (const char* text : instructions) {
            SDL_Surface* surface = TTF_RenderText_Blended(font, text, color);
            SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
            SDL_Rect dstRect = {SCREEN_WIDTH/2, y, surface->w, surface->h};

            SDL_RenderCopy(renderer, texture, nullptr, &dstRect);

            SDL_FreeSurface(surface);
            SDL_DestroyTexture(texture);

            y += 30; // Khoảng cách giữa các dòng
        }
        SDL_RenderPresent(renderer);

            while (SDL_PollEvent(&event)) {
                if (event.type == SDL_QUIT) {
                    running = false;
                    renderInstruction = false;
                    return;
                } else if (event.type == SDL_KEYDOWN) {
                    switch (event.key.keysym.sym) {
                        case SDLK_ESCAPE:
                            renderInstruction = false;
                            showMenu();
                            break;
                    }
                }
            }
        SDL_Delay(100);
    }
}
void Game::initMode(GameMode mode){
    if(state != MENU && state != GAME_OVER && state != WIN) return;
    enemies.clear();
    hearts.clear();
    gameMap.walls.clear();
    gameMap.waters.clear();
    gameMap.bushs.clear();
    gameMap.stones.clear();
    gameMap.ices.clear();
    gateOut.active = false;
    currentBoss = NULL;
    if(level == 4){
        state = SHOW_WINNER;
        return;
    }

    player1.imgLink = "Image//player1.png";
    player2.imgLink = "Image//player2.png";
    player1 = PlayerTank(TILE_SIZE*10 - 1, MAP_HEIGHT * TILE_SIZE - 1, renderer, player1.imgLink);
    base = Base((MAP_WIDTH / 2)*TILE_SIZE, (MAP_HEIGHT - 1)*TILE_SIZE, renderer);
    string filename;

    if(mode == GameMode::PVP){
        if(state == WIN){
            level++;
        }
        filename = "gameMaps//" + to_string(level) + ".txt";
        gameMap.loadFromFile(filename, renderer);
        spawnBoss(level);
        player2 = PlayerTank(TILE_SIZE * 15, MAP_HEIGHT * TILE_SIZE - 1, renderer, player2.imgLink);
        walls = gameMap.walls;
        waters = gameMap.waters;
        bushs = gameMap.bushs;
        stones = gameMap.stones;
        ices = gameMap.ices;
        spawnHearts();
        if(!currentBoss) spawnEnemies();
    }
    else{
        if(state == WIN){
            level++;
        }
        else if(state == GAME_OVER){
            level = 0;
        }
        filename = "gameMaps//" + to_string(level) + ".txt";
        spawnBoss(level);
        gameMap.loadFromFile(filename, renderer);
        walls = gameMap.walls;
        waters = gameMap.waters;
        bushs = gameMap.bushs;
        stones = gameMap.stones;
        ices = gameMap.ices;
        spawnHearts();
        if(!currentBoss) spawnEnemies();
    }
    state = PLAYING;
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
void Game::spawnEnemies() {
    enemies.clear();
    while(enemies.size() < enemyNumber){
        int ex, ey;
        bool validPosition = false;
        while (!validPosition) {
            ex = (2 + (rand() % (MAP_WIDTH - 1))) * TILE_SIZE;
            ey = (2 + rand() % (MAP_HEIGHT / 2) ) * TILE_SIZE;
            SDL_Rect enemyRect = { ex, ey, TILE_SIZE, TILE_SIZE };
            validPosition = true;
            for (const auto& wall : walls) {
                if (wall.active && SDL_HasIntersection(&enemyRect, &wall.rect)) {
                    validPosition = false;
                }
            }
            for (auto& stone : stones) {
                if (SDL_HasIntersection(&enemyRect, &stone.rect)) {
                    validPosition = false;
                }
            }
        }
        enemies.push_back(EnemyTank(ex, ey, renderer));
    }
}

void Game::spawnBoss(int level){
    switch(level){
        case 3:
            //currentBoss = make_unique<FireBoss>((MAP_WIDTH-5) * TILE_SIZE, 5 * TILE_SIZE, renderer);
            currentBoss = new FireBoss((MAP_WIDTH-5) * TILE_SIZE, 5 * TILE_SIZE, renderer);
            AudioManager::PlaySound(1, "fireboss", -1);
            break;
        case 4:
            //currentBoss = make_unique<IceBoss>(3 * TILE_SIZE, 5 * TILE_SIZE, renderer);
            currentBoss = new IceBoss(3 * TILE_SIZE, 5 * TILE_SIZE, renderer);
            AudioManager::PlaySound(2, "iceboss", -1);
            Mix_Volume(2, MIX_MAX_VOLUME / 6);
            break;
        default:
            currentBoss = NULL;
            return;
    }
}

void Game::renderPauseMenu(){
    bool inPauseMenu = true;
    int selectedOption = 0; // 0 = Start Game, 1 = Exit
    SDL_Event event;
    SDL_Color white = {255, 255, 255};
    SDL_Color yellow = {255, 255, 0};
    int highlightAlpha = 128;
    bool alphaIncreasing = true;
    Uint32 lastTime = SDL_GetTicks();
    int lineSpacing = 40;

    while (inPauseMenu) {
        // Vẽ menu
        SDL_RenderClear(renderer);// 👈 vẽ menu
        SDL_RenderCopy(renderer, menuTexture, nullptr, nullptr); // Hiển thị ảnh nền
        //SDL_RenderPresent(renderer);

        Uint32 currentTime = SDL_GetTicks();
        if (currentTime > lastTime + 30) { // cập nhật mỗi ~30ms
            if (alphaIncreasing) {
                highlightAlpha += 8;
                if (highlightAlpha >= 192) {
                    highlightAlpha = 192;
                    alphaIncreasing = false;
                }
            } else {
                highlightAlpha -= 8;
                if (highlightAlpha <= 64) {
                    highlightAlpha = 64;
                    alphaIncreasing = true;
                }
            }
            lastTime = currentTime;
        }

        // T?o các chuỗi menu
        string options[3] = {"Continue", "MENU", "Exit"};
        int totalMenuHeight = 0;
        for (int i = 0; i < 3; ++i) {
            TTF_Font* currentFont = (i == selectedOption) ? fontLarge : font;
            int w, h;
            TTF_SizeText(currentFont, options[i].c_str(), &w, &h);
            totalMenuHeight += h + lineSpacing;
        }
        totalMenuHeight -= lineSpacing; // không tính khoảng sau dòng cuối
        int yOffset = (SCREEN_HEIGHT - totalMenuHeight) / 2 + 200;

        for (int i = 0; i < 3; ++i) {
            SDL_Color color = (i == selectedOption) ? yellow : white;
            TTF_Font* currentFont = (i == selectedOption) ? fontLarge : font;

            SDL_Surface* surface = TTF_RenderText_Blended(currentFont, options[i].c_str(), color);
            SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
            //SDL_Rect dstRect = { SCREEN_WIDTH / 2 - surface->w / 2, 200 + i * 50, surface->w, surface->h };
            SDL_Rect dstRect = {
                SCREEN_WIDTH / 2 - surface->w / 2,
                yOffset,
                surface->w,
                surface->h
            };

            // Highlight động cho lựa chọn đang được chọn
            if (i == selectedOption) {
                SDL_Rect highlightRect = {
                    dstRect.x - 10,
                    dstRect.y - 5,
                    dstRect.w + 20,
                    dstRect.h + 10
                };
                SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
                SDL_SetRenderDrawColor(renderer, 119, 139, 153, highlightAlpha);
                SDL_RenderFillRect(renderer, &highlightRect);
            }

            SDL_RenderCopy(renderer, texture, NULL, &dstRect);
            SDL_FreeSurface(surface);
            SDL_DestroyTexture(texture);

            yOffset += dstRect.h + lineSpacing;

        }

        SDL_RenderPresent(renderer);

        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                inPauseMenu = false;
                running = false;
                return;
            }
            else if (event.type == SDL_KEYDOWN) {
                switch (event.key.keysym.sym) {
                    case SDLK_UP:
                        selectedOption = (selectedOption - 1 + 3) % 3;
                        break;
                    case SDLK_DOWN:
                        selectedOption = (selectedOption + 1) % 3;
                        break;
                    case SDLK_RETURN:
                        if (selectedOption == 0) {
                            state = PLAYING;
                            inPauseMenu = false;
                        } else if (selectedOption == 1) {
                            // Back to Menu
                            state = MENU;
                            inPauseMenu = false;
                        } else if (selectedOption == 2) {
                            // Quit Game
                            running = false;
                            inPauseMenu = false;
                        }
                        break;
                }
            }
        }

        SDL_Delay(16); // tránh dùng 100% CPU
    }
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

