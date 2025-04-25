#include "Boss.h"

FireBoss::FireBoss(int x, int y, SDL_Renderer* renderer) : Boss(x, y) {
    texture = IMG_LoadTexture(renderer, "Image//fire_monster_chosen.png");
    fireTexture = IMG_LoadTexture(renderer, "Image//flame.png");
    //fireZone = FireZone((MAP_WIDTH - 1)*TILE_SIZE, (MAP_HEIGHT-3) * TILE_SIZE, fireTexture);
    name = "FireBoss";
}
void FireBoss::update() {
        Uint32 currentTime = SDL_GetTicks();
        if (currentTime - lastFireTime >= fireInterval) {
            spawnFireZone();
            lastFireTime = currentTime;
        }

        // Cập nhật và loại bỏ các vùng lửa đã hết thời gian
        fireZones.erase(std::remove_if(fireZones.begin(), fireZones.end(),
            [](const unique_ptr<FireZone>& zone) {
                return zone->isExpired();
            }), fireZones.end());
    return;
}

void FireBoss::render(SDL_Renderer* renderer) {
        Uint32 now = SDL_GetTicks();
        if (now - lastFrameTime >= FRAME_DURATION) {
            currentFrame = (currentFrame + 1) % FRAME_COUNT;
            lastFrameTime = now;
        }

        SDL_Rect srcRect = {
            currentFrame * FRAME_WIDTH, //x góc trái trên
            0, //y góc trái trên
            FRAME_WIDTH, //CR frame
            FRAME_HEIGHT //CC frame
        };
        SDL_RenderCopy(renderer, texture, &srcRect, &destRect);

        // Vẽ các vùng lửa
        for (const auto& zone : fireZones) {
            zone->render(renderer);
        }
}

bool FireBoss::checkCollision(PlayerTank& player) {
    for(int i=0; i < fireZones.size(); i++){
        if(SDL_HasIntersection(&player.rect, &fireZones[i]->rect))
            return true;
    }
    return false;
}

void FireBoss::spawnFireZone() {
    for(int i=0; i<15; i++){
        int x = rand() % ((MAP_WIDTH - 5) * TILE_SIZE);
        int y = rand() % ((MAP_HEIGHT - 5) *TILE_SIZE);
        fireZones.push_back(make_unique<FireZone>(x, y, fireTexture));
    }
}


