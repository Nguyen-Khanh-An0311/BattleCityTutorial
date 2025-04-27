#include "Boss_Juior.h"

/*BossJR::BossJR(int startX, int startY, SDL_Renderer* tex) {
        moveDelay = 10; // Delay for movement
        x = startX;
        y = startY;
        angle = 0;
        rect = {x, y, TILE_SIZE, TILE_SIZE};
        bossJRTexture = text;
        dirX = 0;
        dirY = 1;
        active = true;
    }

void BossJR::move(const vector<Wall>& walls, SDL_Renderer* renderer,
                     vector<Stone>& stones, vector<Bush>& bushs, vector<Water>& waters,
                     Base base) {
    if (--moveDelay > 0) return;
    moveDelay = 50;


    if (abs(base.x - x) > abs(base.y - y)) {
        // Ưu tiên trục ngang
        dirX = (base.x > x) ? 20 : -20;
        dirY = 0;
    } else {
        // Ưu tiên trục dọc
        dirY = (base.y > y) ? 20 : -20;
        dirX = 0;
    }

    // Cập nhật hướng và góc
    if (dirX > 0) angle = 270;
    else if (dirX < 0) angle = 90;
    else if (dirY > 0) angle = 0;
    else angle = 180;

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


void BossJR::render(SDL_Renderer* renderer) {
        /*Uint32 now = SDL_GetTicks();
        if (now - lastFrameTime >= BJR_FRAME_DURATION) {
            currentFrame = (currentFrame + 1) % F_FRAME_COUNT;
            lastFrameTime = now;
        }

        SDL_Rect srcRect = {
            currentFrame * F_FRAME_WIDTH, //x góc trái trên
            0, //y góc trái trên
            F_FRAME_WIDTH, //CR frame
            F_FRAME_HEIGHT //CC frame
        };
        SDL_RenderCopy(renderer, bossJRTexture, &srcRect, &rect);

        // Vẽ các vùng lửa
        for (const auto& zone : fireZones) {
            zone->render(renderer);
        }
    if (active) {
        SDL_RenderCopyEx(renderer, bossJRTexture, nullptr, &rect, angle, nullptr, SDL_FLIP_NONE);
    }
}*/

