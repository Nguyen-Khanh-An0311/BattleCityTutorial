#include "EnemyTank.h"

EnemyTank::EnemyTank(int startX, int startY, SDL_Renderer* renderer) {
        moveDelay = 100; // Delay for movement
        shootDelay = 5; // Delay for shooting
        x = startX;
        y = startY;
        angle = 0;
        rect = {x, y, TILE_SIZE, TILE_SIZE};
        tankTexture = IMG_LoadTexture(renderer, "Image//enemy_up.png");
        dirX = 0;
        dirY = 1;
        active = true;
    }

/*void EnemyTank::move(const vector<Wall>& walls, SDL_Renderer* renderer,
                     vector<Stone>& stones, vector<Bush>& bushs, vector<Water>& waters) {
    if (--moveDelay > 0) return;
    moveDelay = 100;
    int r = rand() % 4;
    if (r == 0) { // Up
        this->dirX = 0;
        this->dirY = -40;
        //tankTexture = IMG_LoadTexture(renderer, "enemy_up.png");
    }
    else if (r == 1) { // Down
        this->dirX = 0;
        this->dirY = 40;
        //tankTexture = IMG_LoadTexture(renderer, "enemy_down.png");
    }
    else if (r == 2) { // Left
        this->dirY = 0;
        this->dirX = -40;
        //tankTexture = IMG_LoadTexture(renderer, "enemy_left.png");
    }
    else if (r == 3) { // Right
        this->dirY = 0;
        this->dirX = 40;
        //tankTexture = IMG_LoadTexture(renderer, "enemy_right.png");
    }
    if (dirX > 0) angle = 90;
    else if (dirX < 0) angle = 270;
    else if (dirY > 0) angle = 180;
    else angle = 0;

    int newX = x + dirX;
    int newY = y + dirY;

    SDL_Rect newRect = { newX, newY, TILE_SIZE, TILE_SIZE };
    for (int i = 0; i < walls.size(); i++) {
        if (walls[i].active && SDL_HasIntersection(&newRect, &walls[i].rect)) {
            return; // Prevent movement if colliding with a wall
        }
    }
    for (int i = 0; i < stones.size(); i++) {
        if (SDL_HasIntersection(&newRect, &stones[i].rect)) {
            return; // Prevent movement if colliding with a wall
        }
    }
    for (int i = 0; i < waters.size(); i++) {
        if (SDL_HasIntersection(&newRect, &waters[i].rect)) {
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


}*/

void EnemyTank::move(const vector<Wall>& walls, SDL_Renderer* renderer,
                     vector<Stone>& stones, vector<Bush>& bushs, vector<Water>& waters,
                     PlayerTank& player) {
    if (--moveDelay > 0) return;
    moveDelay = 50;


    if (abs(player.x - x) > abs(player.y - y)) {
        // Ưu tiên trục ngang
        dirX = (player.x > x) ? 20 : -20;
    } else {
        // Ưu tiên trục dọc
        dirY = (player.y > y) ? 20 : -20;
    }

    // Cập nhật hướng và góc
    if (dirX > 0) angle = 90;
    else if (dirX < 0) angle = 270;
    else if (dirY > 0) angle = 180;
    else angle = 0;

    // Tính vị trí mới
    int newX = x + dirX;
    int newY = y + dirY;
    SDL_Rect newRect = { newX, newY, TILE_SIZE, TILE_SIZE };

    // Kiểm tra va chạm tường
    for (int i = 0; i < walls.size(); ++i) {
        if (walls[i].active && SDL_HasIntersection(&newRect, &walls[i].rect)) {
            return;
        }
    }

    // Kiểm tra va chạm đá
    for (int i = 0; i < stones.size(); ++i) {
        if (SDL_HasIntersection(&newRect, &stones[i].rect)) {
            return;
        }
    }

    // Kiểm tra va chạm nước
    for (int i = 0; i < waters.size(); ++i) {
        if (SDL_HasIntersection(&newRect, &waters[i].rect)) {
            return;
        }
    }

    // Nếu hợp lệ, cập nhật vị trí
    if (newX >= 0 && newX + TILE_SIZE <= SCREEN_WIDTH &&
        newY >= 0 && newY + TILE_SIZE <= SCREEN_HEIGHT) {
        x = newX;
        y = newY;
        rect.x = x;
        rect.y = y;
    }
}


void EnemyTank::shoot(SDL_Renderer* renderer) {
    if (--shootDelay > 0) return;
    shootDelay = 5;
    bullets.push_back(Bullet(x + TILE_SIZE / 2 - 5, y + TILE_SIZE / 2 - 5,
                             this->dirX, this->dirY, renderer));
}

void EnemyTank::updateBullets() {
    for (auto &bullet : bullets) {
        bullet.move();
    }
    bullets.erase(std::remove_if(bullets.begin(), bullets.end(),
                 [](Bullet &b) { return !b.active; }), bullets.end());
}

void EnemyTank::render(SDL_Renderer* renderer) {
    //SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    //SDL_RenderFillRect(renderer, &rect);
    SDL_RenderCopyEx(renderer, tankTexture, nullptr, &rect, angle, nullptr, SDL_FLIP_NONE);
    for (auto &bullet : bullets) {
        bullet.render(renderer);
    }
}

