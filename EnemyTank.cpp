#include "EnemyTank.h"

EnemyTank::EnemyTank(int startX, int startY) {
        moveDelay = 100; // Delay for movement
        shootDelay = 5; // Delay for shooting
        x = startX;
        y = startY;
        rect = {x, y, TILE_SIZE, TILE_SIZE};
        dirX = 0;
        dirY = 1;
        active = true;
    }

void EnemyTank::move(const vector<Wall>& walls) {
    if (--moveDelay > 0) return;
    moveDelay = 100;
    int r = rand() % 4;
    if (r == 0) { // Up
        this->dirX = 0;
        this->dirY = -20;
    }
    else if (r == 1) { // Down
        this->dirX = 0;
        this->dirY = 20;
    }
    else if (r == 2) { // Left
        this->dirY = 0;
        this->dirX = -20;
    }
    else if (r == 3) { // Right
        this->dirY = 0;
        this->dirX = 20;
    }

    int newX = x + dirX;
    int newY = y + dirY;
    /*this->dirX = dx;
    this->dirY = dy;*/

    SDL_Rect newRect = { newX, newY, TILE_SIZE, TILE_SIZE };
    for (int i = 0; i < walls.size(); i++) {
        if (walls[i].active && SDL_HasIntersection(&newRect, &walls[i].rect)) {
            return; // Prevent movement if colliding with a wall
        }
    }

    if (newX >= TILE_SIZE && newX <= SCREEN_WIDTH - TILE_SIZE * 2 &&
        newY >= TILE_SIZE && newY <= SCREEN_HEIGHT - TILE_SIZE * 2) {
        x = newX;
        y = newY;
        rect.x = x;
        rect.y = y;
    }
}

void EnemyTank::shoot() {
    if (--shootDelay > 0) return;
    shootDelay = 5;
    bullets.push_back(Bullet(x + TILE_SIZE / 2 - 5, y + TILE_SIZE / 2 - 5,
                             this->dirX, this->dirY));
}

void EnemyTank::updateBullets() {
    for (auto &bullet : bullets) {
        bullet.move();
    }
    bullets.erase(std::remove_if(bullets.begin(), bullets.end(),
                 [](Bullet &b) { return !b.active; }), bullets.end());
}

void EnemyTank::render(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderFillRect(renderer, &rect);
    for (auto &bullet : bullets) {
        bullet.render(renderer);
    }
}

