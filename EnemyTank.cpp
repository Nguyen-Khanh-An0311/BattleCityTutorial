#include "EnemyTank.h"

EnemyTank::EnemyTank(){}
EnemyTank::EnemyTank(int startX, int startY, SDL_Renderer* renderer) {
        moveDelay = 2; // Delay for movement
        shootDelay = 3; // Delay for shooting
        x = startX;
        y = startY;
        angle = 0;
        rect = {x, y, TILE_SIZE , TILE_SIZE };
        tankTexture = IMG_LoadTexture(renderer, "Image//enemy.png");
        dirX = 0;
        dirY = 1;
        active = true;
    }

bool EnemyTank::isBlocked(SDL_Rect rect, vector<Stone>& stones, vector<Water>& waters) {
    for (auto& s : stones) {
        if (SDL_HasIntersection(&rect, &s.rect)) return true;
    }
    for (auto& wa : waters) {
        if (SDL_HasIntersection(&rect, &wa.rect)) return true;
    }
    return false;
}

void EnemyTank::move(vector<Wall>& walls, SDL_Renderer* renderer,
                     vector<Stone>& stones, vector<Bush>& bushs, vector<Water>& waters,
                     Base base) {
    if(--moveDelay > 0) return;
    moveDelay = 2;

    /*if (abs(base.x - x) > abs(base.y - y)) {
        // Ưu tiên trục ngang
        dirX = (base.x > x) ? 5 : -5;
        dirY = 0;
    } else {
        // Ưu tiên trục dọc
        dirY = (base.y > y) ? 5 : -5;
        dirX = 0;
    }*/
    if (y < MAP_HEIGHT * TILE_SIZE){
        dirX = 0;
        dirY = 5;
    }
    else {
        dirX = (base.x > x) ? 5 : -5;
        dirY = 0;
    }

    // Tính vị trí mới
    int newX = x + dirX;
    int newY = y + dirY;
    SDL_Rect newRect = { newX, newY, TILE_SIZE, TILE_SIZE};
    /*if (isBlocked(newRect, stones, waters)) {
        bool found = false;
        // Bị chắn bởi đá hoặc gì đó, thử hướng khác
        const int directions[4][2] = { {5,0}, {-5,0}, {0,5}, {0,-5} };
        for (auto& d : directions) {
            SDL_Rect tryRect = { x + d[0], y + d[1], TILE_SIZE, TILE_SIZE };
            if (!isBlocked(tryRect, stones, waters)) {
                dirX = d[0];
                dirY = d[1];
                newX = x + dirX;
                newY = y + dirY;
                newRect = {newX, newY, TILE_SIZE, TILE_SIZE};
                found = true;
                break;
            }
        }
        if(!found) {
            dirX = 0;
            dirY = 0;
            newX = x;
            newY = y;
            newRect = {newX, newY, TILE_SIZE, TILE_SIZE};
        }
    }*/
    if (isBlocked(newRect, stones, waters)) {
        vector<pair<int, int>> possibleDirs;
        int directions[4][2] = { {5, 0}, {-5, 0}, {0, 5}, {0, -5} };

        for (auto& d : directions) {
            SDL_Rect tryRect = { x + d[0], y + d[1], TILE_SIZE, TILE_SIZE };
            if (!isBlocked(tryRect, stones, waters)) {
                possibleDirs.emplace_back(d[0], d[1]);
            }
        }

        if (!possibleDirs.empty()) {
            auto bestDir = *std::min_element(possibleDirs.begin(), possibleDirs.end(),
                [&](const pair<int, int>& a, const pair<int, int>& b) {
                    int ax = x + a.first * TILE_SIZE;
                    int ay = y + a.second * TILE_SIZE;
                    int bx = x + b.first * TILE_SIZE;
                    int by = y + b.second * TILE_SIZE;
                    int da = abs(ax - base.x);
                    int db = abs(bx - base.x);
                    return da + abs(ay - base.y) < db + abs(by - base.y);
                });

            dirX = bestDir.first;
            dirY = bestDir.second;
            newX = x + dirX;
            newY = y + dirY;
            newRect = { newX, newY, TILE_SIZE, TILE_SIZE };
        } else {
            // Không có hướng nào khả thi → đứng yên
            dirX = 0;
            dirY = 0;
            newX = x;
            newY = y;
            newRect = { x, y, TILE_SIZE, TILE_SIZE };
        }
    }

    if (dirX > 0) angle = 270;
    else if (dirX < 0) angle = 90;
    else if (dirY > 0) angle = 0;
    else if (dirY < 0) angle = 180;


    // Kiểm tra va chạm tường
    for (int i = 0; i < walls.size(); ++i) {
        if (walls[i].active && SDL_HasIntersection(&newRect, &walls[i].rect)) {
            return;
        }
    }
    if(SDL_HasIntersection(&base.rect, &newRect)) return;



    // Nếu hợp lệ, cập nhật vị trí
    if (newX >= TILE_SIZE && newX <= MAP_WIDTH * TILE_SIZE &&
        newY >= TILE_SIZE && newY <= MAP_HEIGHT * TILE_SIZE) {
        x = newX;
        y = newY;
        rect.x = x;
        rect.y = y;
    }
}



void EnemyTank::shoot(SDL_Renderer* renderer) {
    if (--shootDelay > 0) return;
    shootDelay = 3;
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
    SDL_RenderCopyEx(renderer, tankTexture, nullptr, &rect, angle, nullptr, SDL_FLIP_NONE);
    for (auto &bullet : bullets) {
        bullet.render(renderer);
    }
}

