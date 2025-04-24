#include "Boss.h"

FireBoss::FireBoss(int x, int y, SDL_Renderer* renderer) : Boss(x, y) {
        // Load ảnh riêng
    texture = IMG_LoadTexture(renderer, "Image//fire_monster_chosen.png");
    name = "FireBoss";
}
void FireBoss::update() {
    // Hành vi tấn công bắn đạn lửa
    //cout << "FireBoss update\n";
    return;
}
void FireBoss::render(SDL_Renderer* renderer) {
        Uint32 now = SDL_GetTicks();
        if (now - lastFrameTime >= FRAME_DURATION) {
            currentFrame = (currentFrame + 1) % FRAME_COUNT;
            lastFrameTime = now;
        }

        SDL_Rect srcRect = { currentFrame * FRAME_WIDTH, 0, FRAME_WIDTH, FRAME_HEIGHT };
        SDL_RenderCopy(renderer, texture, &srcRect, &destRect);
        SDL_Delay(16);// dieu chinh fps = 60
}
