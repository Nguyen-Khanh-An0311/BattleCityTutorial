#include "Boss.h"

FireBoss::FireBoss(int x, int y, SDL_Renderer* renderer) : Boss(x, y) {
    texture = IMG_LoadTexture(renderer, "Image//fire_monster_chosen.png");
    fireTexture = IMG_LoadTexture(renderer, "Image//flame.png");
    fireSound = Mix_LoadWAV("Sound//flameSound.wav");
    bossSound = Mix_LoadWAV("Sound//bossSound.wav");
    bossDie = IMG_LoadTexture(renderer, "Image//fire_boss_die.png");
    name = "FireBoss";
}
void FireBoss::update() {
        Uint32 currentTime = SDL_GetTicks();
        if (currentTime - lastFireTime >= fireInterval) {
            Mix_PlayChannel(-1, fireSound, 0);
            Mix_VolumeChunk(fireSound, MIX_MAX_VOLUME / 4);
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
        Mix_PlayChannel(-1, bossSound, -1);
        Mix_VolumeChunk(bossSound, MIX_MAX_VOLUME / 4);
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
void FireBoss::Die(SDL_Renderer* renderer) {
    if (!isDying) {
        // lần đầu chết
        isDying = true;
        currentFrame_die = 0;
        lastFrameTime_die = SDL_GetTicks();
    }

    Uint32 now = SDL_GetTicks();
    if (now - lastFrameTime_die >= FRAME_DURATION) {
        currentFrame_die++;
        lastFrameTime_die = now;
    }

    if (currentFrame_die >= DIE_FRAME_COUNT) {
        active = false; // xong animation thì biến mất
        return;
    }

    SDL_Rect srcRect = {
        currentFrame_die * FRAME_WIDTH,
        0,
        FRAME_WIDTH,
        FRAME_HEIGHT
    };
    SDL_RenderCopy(renderer, bossDie, &srcRect, &destRect);
}


IceBoss::IceBoss(int x, int y, SDL_Renderer* renderer) : Boss(x, y) {
    texture = IMG_LoadTexture(renderer, "Image//ice_monster_chosen.png");
    //fireTexture = IMG_LoadTexture(renderer, "Image//flame.png");
    //fireSound = Mix_LoadWAV("Sound//flameSound.wav");
    bossSound = Mix_LoadWAV("Sound//bossSound.wav");
    //bossDie = IMG_LoadTexture(renderer, "Image//fire_boss_die.png");
    name = "IceBoss";
}
void IceBoss::update(){
    return;
}
void IceBoss::render(SDL_Renderer* renderer) {
        Mix_PlayChannel(-1, bossSound, -1);
        Mix_VolumeChunk(bossSound, MIX_MAX_VOLUME / 4);
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
}
