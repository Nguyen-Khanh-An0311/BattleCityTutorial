#include "PlayerTank.h"
using namespace std;


PlayerTank::PlayerTank(int startX, int startY, SDL_Renderer* renderer) {
        RemainingLives = 3;
        x = startX;
        y = startY;
        rect = {x, y, TILE_SIZE, TILE_SIZE}; // Vị trí và kích thước xe tăng
        tankTexture = IMG_LoadTexture(renderer, "player_up.png");

        dirX = 0;
        dirY = -1; // Default direction up
    }
PlayerTank::PlayerTank(){};

void PlayerTank::shoot() {
    bullets.push_back(Bullet(x + TILE_SIZE / 2 - 5, y + TILE_SIZE / 2 - 5,
        this->dirX, this->dirY));
}

void PlayerTank::updateBullets() {
    for (auto &bullet : bullets) {
        bullet.move();
    }
    bullets.erase(remove_if(bullets.begin(), bullets.end(),
        [](Bullet &b) { return !b.active; }), bullets.end());
}

void PlayerTank::move(int dx, int dy, const vector<Wall>& walls, vector<Heart>& hearts, vector<EnemyTank>& enemies) {
    int newX = x + dx;
    int newY = y + dy;
    this->dirX = dx;
    this->dirY = dy;

    SDL_Rect newRect = { newX, newY, TILE_SIZE, TILE_SIZE };
    for (int i = 0; i < walls.size(); i++) {
        if (walls[i].active && SDL_HasIntersection(&newRect, &walls[i].rect)) {
            return; // Prevent movement if colliding with a wall
        }
    }

    for (int i = 0; i < enemies.size(); i++) {
        if (enemies[i].active && SDL_HasIntersection(&newRect, &enemies[i].rect)) {
            return; // Prevent movement if colliding with a wall
        }
    }

    if (newX >= TILE_SIZE && newX <= SCREEN_WIDTH  &&
        newY >= TILE_SIZE && newY <= SCREEN_HEIGHT ) {
        x = newX;
        y = newY;
        rect.x = x;
        rect.y = y;
    }

    for(int i=0; i<hearts.size(); i++){
        if (hearts[i].active && SDL_HasIntersection(&newRect, &hearts[i].rect) ){
            hearts[i].active = false;
            RemainingLives += 1;
            hearts.erase(hearts.begin() + i);
            break;
        }
    }
}

void PlayerTank::render(SDL_Renderer* renderer){
    //SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
    //SDL_RenderFillRect(renderer, &rect);
    SDL_RenderCopy(renderer, tankTexture, nullptr, &rect);
    for (auto &bullet : bullets) {
        bullet.render(renderer);
    }
}
