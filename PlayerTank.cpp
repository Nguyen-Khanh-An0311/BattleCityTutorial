/*#include "PlayerTank.h"
using namespace std;


PlayerTank::PlayerTank(int startX, int startY, SDL_Renderer* renderer, const char* imagePath, const char* spawnPath) {
        RemainingLives = 3;
        x = startX;
        y = startY;
        rect = {x, y, TILE_SIZE - 4, TILE_SIZE - 4};
        angle = 0;
        SDL_Surface* tempSurface = IMG_Load(imagePath);
        //SDL_Surface* tempSurface2 = IMG_Load(spawnPath);

        // Set colorkey để làm trong suốt nền đen
        SDL_SetColorKey(tempSurface, SDL_TRUE, SDL_MapRGB(tempSurface->format, 0, 0, 0));

        // Tạo texture từ surface sau khi đã xử lý colorkey
        tankTexture = SDL_CreateTextureFromSurface(renderer, tempSurface);
        //spawnTexture = SDL_CreateTextureFromSurface(renderer, tempSurface2);
        SDL_FreeSurface(tempSurface);
        //SDL_FreeSurface(tempSurface2);

        // Blend để hỗ trợ trong suốt
        SDL_SetTextureBlendMode(tankTexture, SDL_BLENDMODE_BLEND);
        //SDL_SetTextureBlendMode(spawnTexture, SDL_BLENDMODE_BLEND);


        dirX = 0;
        dirY = -1; // Default direction up
        cooldown = 0;
        score = 0;
        active = true;
        //state = Spawning;
        spawnStartTime = SDL_GetTicks();
    }

PlayerTank::PlayerTank(){};

void PlayerTank::shoot(SDL_Renderer* renderer) {
    if(doneSpawn())
        bullets.push_back(Bullet(x + TILE_SIZE / 2 - 5, y + TILE_SIZE / 2 - 5,
        this->dirX, this->dirY, renderer));
}

void PlayerTank::updateBullets() {
    if(doneSpawn()){
        for (auto &bullet : bullets) {
            bullet.move();
        }
        bullets.erase(remove_if(bullets.begin(), bullets.end(),
            [](Bullet &b) { return !b.active; }), bullets.end());
    }
}

void PlayerTank::move(int dx, int dy, const vector<Wall>& walls, vector<Heart>& hearts, vector<EnemyTank>& enemies,
                      vector<Stone>& stones, vector<Bush>& bushs, vector<Water>& waters) {
    if(doneSpawn()){
        if(active){
            int newX = x + dx;
            int newY = y + dy;
            this->dirX = dx;
            this->dirY = dy;

            if (dx >0) angle = 90;
            else if (dx <0) angle = 270;
            else if (dy >0) angle = 180;
            else angle = 0;

            SDL_Rect newRect = { newX, newY, TILE_SIZE - 4, TILE_SIZE - 4};
            for (int i = 0; i < walls.size(); i++) {
                if (walls[i].active && SDL_HasIntersection(&newRect, &walls[i].rect)) {
                    return; // Prevent movement if colliding with a wall
                }
            }
            for (int i = 0; i < stones.size(); i++) {
                if (SDL_HasIntersection(&newRect, &stones[i].rect)) {
                    return;
                }
            }
            for (int i = 0; i < waters.size(); i++) {
                if (SDL_HasIntersection(&newRect, &waters[i].rect)) {
                    return;
                }
            }

            for (int i = 0; i < enemies.size(); i++) {
                if (enemies[i].active && SDL_HasIntersection(&newRect, &enemies[i].rect)) {
                    return; // Prevent movement if colliding with a wall
                }
            }

            if (newX >= TILE_SIZE && newX <= MAP_WIDTH * TILE_SIZE  &&
                newY >= TILE_SIZE && newY <= MAP_HEIGHT * TILE_SIZE ) {
                x = newX;
                y = newY;
                rect.x = x;
                rect.y = y;
            }

            for(int i=0; i<hearts.size(); i++){
                if (hearts[i].active && SDL_HasIntersection(&newRect, &hearts[i].rect) ){
                    hearts[i].active = false;
                    RemainingLives += 1;
                    //hearts.erase(hearts.begin() + i);
                    break;
                }
            }
        }
    }
}

bool PlayerTank::doneSpawn(){
    return SDL_GetTicks() - spawnStartTime > SPAWN_DURATION;
}

void PlayerTank::render(SDL_Renderer* renderer){
    if(!doneSpawn()){
        Uint32 now = SDL_GetTicks();
        if (now - lastFrameTime >= SPAWN_FRAME_DURATION) {
            currentFrame = (currentFrame + 1) % SPAWN_FRAME_COUNT;
            lastFrameTime = now;
        }
        SDL_Rect srcRect = { currentFrame * SPAWN_FRAME_WIDTH, 0, SPAWN_FRAME_WIDTH, SPAWN_FRAME_HEIGHT };
        SDL_RenderCopy(renderer, spawnTexture, &srcRect, &rect);
    }
    else {
        if(active){
            SDL_RenderCopyEx(renderer, tankTexture, nullptr, &rect, angle, nullptr, SDL_FLIP_NONE);
            for (auto &bullet : bullets) {
                bullet.render(renderer);
            }
        }
    }
}*/

#include "PlayerTank.h"
using namespace std;


PlayerTank::PlayerTank(int startX, int startY, SDL_Renderer* renderer, const char* imagePath) {
        RemainingLives = 3;
        x = startX;
        y = startY;
        rect = {x, y, TILE_SIZE, TILE_SIZE};
        angle = 0;
        SDL_Surface* tempSurface = IMG_Load(imagePath);
        if (!tempSurface) {
            printf("Failed to load surface: %s\n", IMG_GetError());
        }

        // Set colorkey để làm trong suốt nền đen
        SDL_SetColorKey(tempSurface, SDL_TRUE, SDL_MapRGB(tempSurface->format, 0, 0, 0));

        // Tạo texture từ surface sau khi đã xử lý colorkey
        tankTexture = SDL_CreateTextureFromSurface(renderer, tempSurface);
        SDL_FreeSurface(tempSurface);

        // Blend để hỗ trợ trong suốt
        SDL_SetTextureBlendMode(tankTexture, SDL_BLENDMODE_BLEND);


        dirX = 0;
        dirY = -1; // Default direction up
        cooldown = 0;
        active = true;
    }

PlayerTank::PlayerTank(){};

void PlayerTank::init(int startX, int startY, SDL_Renderer* renderer, const char* imagePath, const char* spawnPath){
        RemainingLives = 3;
        x = startX;
        y = startY;
        rect = {x, y, TILE_SIZE - 2, TILE_SIZE - 2};
        angle = 0;
        SDL_Surface* tempSurface = IMG_Load(imagePath);
        SDL_Surface* tempSurface2 = IMG_Load(spawnPath);
        if (!tempSurface) {
            printf("Failed to load surface: %s\n", IMG_GetError());
        }

        // Set colorkey để làm trong suốt nền đen
        SDL_SetColorKey(tempSurface, SDL_TRUE, SDL_MapRGB(tempSurface->format, 0, 0, 0));

        // Tạo texture từ surface sau khi đã xử lý colorkey
        tankTexture = SDL_CreateTextureFromSurface(renderer, tempSurface);
        spawnTexture = SDL_CreateTextureFromSurface(renderer, tempSurface2);
        SDL_FreeSurface(tempSurface);
        SDL_FreeSurface(tempSurface2);

        // Blend để hỗ trợ trong suốt
        SDL_SetTextureBlendMode(tankTexture, SDL_BLENDMODE_BLEND);


        dirX = 0;
        dirY = -1; // Default direction up
        cooldown = 0;
        spawnStartTime = SDL_GetTicks();
        lastFrameTime = 0;
        currentFrame = 0;
        active = true;
}

bool PlayerTank::doneSpawn(){
    return SDL_GetTicks() - spawnStartTime > SPAWN_DURATION;
}

void PlayerTank::shoot(SDL_Renderer* renderer) {
    if(doneSpawn() && SDL_GetTicks() - lastShotTime > shootDelay){
        bullets.push_back(Bullet(x + TILE_SIZE / 2 - 5, y + TILE_SIZE / 2 - 5,
        this->dirX, this->dirY, renderer));
        lastShotTime = SDL_GetTicks();
    }
}

void PlayerTank::updateBullets() {
    if(doneSpawn()){
        for (auto &bullet : bullets) {
            bullet.move();
        }
        bullets.erase(remove_if(bullets.begin(), bullets.end(),
            [](Bullet &b) { return !b.active; }), bullets.end());
    }
}

void PlayerTank::move(int dx, int dy, const vector<Wall>& walls, vector<Heart>& hearts, vector<EnemyTank>& enemies,
                      vector<Stone>& stones, vector<Bush>& bushs, vector<Water>& waters) {
    if(doneSpawn()){
        if(active){
            int newX = x + dx;
            int newY = y + dy;
            this->dirX = dx;
            this->dirY = dy;

            if (dx >0) angle = 90;
            else if (dx <0) angle = 270;
            else if (dy >0) angle = 180;
            else angle = 0;

            SDL_Rect newRect = { newX, newY, TILE_SIZE - 2 , TILE_SIZE - 2 };
            for (int i = 0; i < walls.size(); i++) {
                if (walls[i].active && SDL_HasIntersection(&newRect, &walls[i].rect)) {
                    return; // Prevent movement if colliding with a wall
                }
            }
            for (int i = 0; i < stones.size(); i++) {
                if (SDL_HasIntersection(&newRect, &stones[i].rect)) {
                    return;
                }
            }
            for (int i = 0; i < waters.size(); i++) {
                if (SDL_HasIntersection(&newRect, &waters[i].rect)) {
                    return;
                }
            }

            for (int i = 0; i < enemies.size(); i++) {
                if (enemies[i].active && SDL_HasIntersection(&newRect, &enemies[i].rect)) {
                    return; // Prevent movement if colliding with a wall
                }
            }

            if (newX >= TILE_SIZE && newX <= (MAP_WIDTH + 1) * TILE_SIZE  &&
                newY >= TILE_SIZE && newY <= (MAP_HEIGHT) * TILE_SIZE ) {
                x = newX;
                y = newY;
                rect.x = x;
                rect.y = y;
            }

            for(int i=0; i<hearts.size(); i++){
                if (hearts[i].active && SDL_HasIntersection(&newRect, &hearts[i].rect) ){
                    hearts[i].active = false;
                    RemainingLives += 1;
                    //hearts.erase(hearts.begin() + i);
                    break;
                }
            }
        }
    }
}

void PlayerTank::render(SDL_Renderer* renderer){
    if(doneSpawn()){
        if(active){
            SDL_RenderCopyEx(renderer, tankTexture, nullptr, &rect, angle, nullptr, SDL_FLIP_NONE);
            for (auto &bullet : bullets) {
                bullet.render(renderer);
            }
        }
    }
    else{
        Uint32 now = SDL_GetTicks();
        if (now - lastFrameTime >= SPAWN_FRAME_DURATION) {
            currentFrame = (currentFrame + 1) % SPAWN_FRAME_COUNT;
            lastFrameTime = now;
        }

        SDL_Rect srcRect = { currentFrame * SPAWN_FRAME_WIDTH, 0, SPAWN_FRAME_WIDTH, SPAWN_FRAME_HEIGHT };
        SDL_RenderCopy(renderer, spawnTexture, &srcRect, &rect);
    }
}
