#include "Boss.h"



FireBoss::FireBoss(int x, int y, SDL_Renderer* renderer) : Boss(x, y) {
    texture = IMG_LoadTexture(renderer, "Image//fire_monster_chosen.png");
    fireTexture = IMG_LoadTexture(renderer, "Image//flame.png");
    //fireSound = Mix_LoadWAV("Sound//flameSound.wav");
    bossDie = IMG_LoadTexture(renderer, "Image//fire_boss_die.png");
    holeTexture = IMG_LoadTexture(renderer, "Image//hole.jpg");
    lastFireTime = SDL_GetTicks();
    lastOpenTime = SDL_GetTicks();
    name = "FireBoss";
}
void FireBoss::update() {
        Uint32 currentTime = SDL_GetTicks();
        if (currentTime - lastFireTime >= fireInterval) {
            fireZones.clear();
            spawnFireZone();
            lastFireTime = currentTime;
        }
        if (!hole && currentTime - lastOpenTime >= holeInterval){
            hole = spawnHole();
        }
        if(hole && hole->isExpired()){
            hole = NULL;
            lastOpenTime = currentTime;
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
        // Vẽ hole
        if(hole && !hole->isExpired()){
            hole->render(renderer);
            hole->spawnEnemies(renderer);
            enemiesFromHole.clear();
            enemiesFromHole.insert(enemiesFromHole.end(), hole->enemies.begin(), hole->enemies.end());
            hole->enemies.clear();
        }
}
bool FireBoss::checkCollision(PlayerTank& player) {
    for(size_t i=0; i < fireZones.size(); i++){
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
Hole* FireBoss::spawnHole(){
    Hole* hole = new Hole((MAP_WIDTH/2 + 1) * TILE_SIZE, (MAP_HEIGHT/2 - 2) * TILE_SIZE, holeTexture);
    return hole;
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
    Mix_HaltChannel(1);
    Mix_FreeChunk(AudioManager::sounds["fireboss"]);
}

IceBoss::IceBoss(int x, int y, SDL_Renderer* renderer) : Boss(x, y) {
    texture = IMG_LoadTexture(renderer, "Image//ice_monster_chosen.png");
    iceTexture = IMG_LoadTexture(renderer, "Image//ice_spike_no_border_fixed.png");
    bossSound = Mix_LoadWAV("Sound//bossSound.wav");
    bossDie = IMG_LoadTexture(renderer, "Image//ice_boss_die.png");
    name = "IceBoss";
}
void IceBoss::update(){
        Uint32 currentTime = SDL_GetTicks();
        if (currentTime - lastIceTime >= iceInterval) {
            spawnIceZone();
            lastIceTime = currentTime;
        }

        // Cập nhật và loại bỏ các vùng lửa đã hết thời gian
        iceZones.erase(std::remove_if(iceZones.begin(), iceZones.end(),
            [](const unique_ptr<IceZone>& zone) {
                return zone->isExpired();
            }), iceZones.end());
    return;
}
void IceBoss::render(SDL_Renderer* renderer) {
        Mix_PlayChannel(-1, bossSound, -1);
        Mix_VolumeChunk(bossSound, MIX_MAX_VOLUME / 4);
        Uint32 now = SDL_GetTicks();
        if (now - lastIceTime >= FRAME_DURATION) {
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

        // Vẽ các vùng băng
        for (const auto& zone : iceZones) {
            zone->render(renderer);
        }
}
void IceBoss::Die(SDL_Renderer* renderer){
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
void IceBoss::spawnIceZone(){
    for(int i=0; i<15; i++){
        int x = (2 + rand() % ((MAP_WIDTH - 3) - 2 + 1)) * TILE_SIZE;
        int y = (2 + rand() % ((MAP_HEIGHT - 3) - 2 + 1)) * TILE_SIZE;
        unique_ptr<IceZone> ice = make_unique<IceZone>(x, y, iceTexture);
        bool validPosiotion = true;
        for(const auto& iceZone : iceZones){
            if(SDL_HasIntersection(&ice->rect, &iceZone->rect)){
                validPosiotion = false;
                break;
            }
        }
        if(validPosiotion) iceZones.push_back(make_unique<IceZone>(x, y, iceTexture));
    }
}
bool IceBoss::checkCollision(PlayerTank& player){
    for(size_t i=0; i < iceZones.size(); i++){
        if(SDL_HasIntersection(&player.rect, &iceZones[i]->rect))
            return true;
    }
    return false;
}
