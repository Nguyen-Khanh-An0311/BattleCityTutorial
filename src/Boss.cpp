#include "Boss.h"


Hole* Boss::spawnHole(SDL_Texture* holeTexture){
    Hole* hole = new Hole(x - TILE_SIZE * 2, y + TILE_SIZE * 2, holeTexture);
    return hole;
}
Shield* Boss::spawnShield(SDL_Texture* shieldTexture){
    Shield* shield = new Shield(x - 10, y - 10, shieldTexture);
    return shield;
}
bool Boss::hasStrongIntersection(const SDL_Rect& a, const SDL_Rect& b, float requiredOverlapRatio = 0.2f) {
    SDL_Rect intersection;
    if (!SDL_IntersectRect(&a, &b, &intersection)) {
        return false;
    }

    int interArea = intersection.w * intersection.h;
    int aArea = a.w * a.h;
    int bArea = b.w * b.h;

    // Kiểm tra nếu phần giao nhau > 50% diện tích của cả hai hình (hoặc một hình tùy bạn)
    float ratioA = (float)interArea / aArea;
    float ratioB = (float)interArea / bArea;

    return (ratioA >= requiredOverlapRatio) && (ratioB >= requiredOverlapRatio);
}
void Boss::renderHP(SDL_Renderer* renderer){
    // Kích thước thanh máu
    int barWidth = 200;
    int barHeight = 10;
    int x = destRect.x; //position.x + (position.w - barWidth) / 2;
    int y = destRect.y - 15; // nằm phía trên boss

    // Thanh nền (xám)
    SDL_Rect bg = { x, y, barWidth, barHeight };
    SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255);
    SDL_RenderFillRect(renderer, &bg);

    // Thanh máu (đỏ)
    int hpWidth = static_cast<int>((double)RemainingLives / fullHP * barWidth);
    SDL_Rect fg = { x, y, hpWidth, barHeight };
    SDL_SetRenderDrawColor(renderer, 200, 0, 0, 255);
    SDL_RenderFillRect(renderer, &fg);
}

FireBoss::FireBoss(int x, int y, SDL_Renderer* renderer) : Boss(x, y, renderer) {
    texture = IMG_LoadTexture(renderer, "Image//fire_monster_chosen.png");
    fireTexture = IMG_LoadTexture(renderer, "Image//flame.png");
    bossDie = IMG_LoadTexture(renderer, "Image//fire_boss_die.png");
    lastFireTime = SDL_GetTicks();
    lastOpenTime = SDL_GetTicks();
    name = "FireBoss";
}
void FireBoss::update() {
        Uint32 currentTime = SDL_GetTicks();
        if (currentTime - lastFireTime >= fireInterval) {
            fireZones.clear();
            spawnFireZone();
            AudioManager::PlaySound(6, "flame", 0);
            lastFireTime = currentTime;
        }
        if (!hole && currentTime - lastOpenTime >= holeInterval){
            hole = Boss::spawnHole(holeTexture);
        }
        if(hole && hole->isExpired()){
            hole = NULL;
            lastOpenTime = currentTime;
        }
        if((!shield || (shield && shield->isExpired())) && ((int)RemainingLives % 2 == 0) && (RemainingLives != lastRemainingLives)){
            shield = Boss::spawnShield(shieldTexture);
            lastRemainingLives = RemainingLives;
        }
        if(shield && shield->isExpired()) shield = NULL;

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
        renderHP(renderer);


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
            //enemiesFromHole = hole->enemies;
            hole->enemies.clear();
        }
        if(shield && !shield->isExpired()){
            shield->render(renderer);
        }
}

bool FireBoss::checkCollision(PlayerTank& player) {
    for(size_t i=0; i < fireZones.size(); i++){
        if(SDL_HasIntersection(&player.rect, &fireZones[i]->rect))
            //fireZones.erase(fireZones.begin() + i);
            return true;
    }
    return false;
}
void FireBoss::spawnFireZone() {
    for(int i=0; i<15; i++){
        int x = rand() % ((MAP_WIDTH - 2) * TILE_SIZE);
        int y = rand() % ((MAP_HEIGHT - 2) *TILE_SIZE);
        fireZones.push_back(make_unique<FireZone>(x, y, fireTexture));
    }
}
void FireBoss::Die(SDL_Renderer* renderer) {
    Mix_HaltChannel(1);
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

IceBoss::IceBoss(int x, int y, SDL_Renderer* renderer) : Boss(x, y, renderer) {
    texture = IMG_LoadTexture(renderer, "Image//ice_monster_chosen.png");
    iceTexture = IMG_LoadTexture(renderer, "Image//ice_spike_no_border_fixed_2.png");
    bossDie = IMG_LoadTexture(renderer, "Image//ice_boss_die.png");
    holeTexture = IMG_LoadTexture(renderer, "Image//hole.jpg");
    lastIceTime = SDL_GetTicks();
    lastOpenTime = SDL_GetTicks();
    name = "IceBoss";
}
void IceBoss::update(){
        Uint32 currentTime = SDL_GetTicks();
        if (currentTime - lastIceTime >= iceInterval) {
            iceZones.clear();
            spawnIceZone();
            AudioManager::PlaySound(5, "ice", 2);
            Mix_Volume(5, MIX_MAX_VOLUME);
            lastIceTime = currentTime;
        }
        if (!hole && currentTime - lastOpenTime >= holeInterval){
            hole = spawnHole(holeTexture);
        }
        if(hole && hole->isExpired()){
            hole = NULL;
            lastOpenTime = currentTime;
        }
        if((!shield || (shield && shield->isExpired())) && ((int)RemainingLives % 2 == 0) && (RemainingLives != lastRemainingLives)){
            shield = Boss::spawnShield(shieldTexture);
            lastRemainingLives = RemainingLives;
        }
        if(shield && shield->isExpired()) shield = NULL;

        // Cập nhật và loại bỏ các vùng lửa đã hết thời gian
        iceZones.erase(std::remove_if(iceZones.begin(), iceZones.end(),
            [](const unique_ptr<IceZone>& zone) {
                return zone->isExpired();
            }), iceZones.end());
    return;
}
void IceBoss::render(SDL_Renderer* renderer) {
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
        renderHP(renderer);

        // Vẽ các vùng băng
        for (const auto& zone : iceZones) {
            zone->render(renderer);
        }
        if(hole && !hole->isExpired()){
            hole->render(renderer);
            hole->spawnEnemies(renderer);
            enemiesFromHole.clear();
            enemiesFromHole.insert(enemiesFromHole.end(), hole->enemies.begin(), hole->enemies.end());
            hole->enemies.clear();
        }
        if(shield && !shield->isExpired()){
            shield->render(renderer);
        }
}
void IceBoss::Die(SDL_Renderer* renderer){
    Mix_HaltChannel(2);
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
        if(hasStrongIntersection(player.rect, iceZones[i]->rect)){
            player.state = FROZEN;
            player.frozenTime = SDL_GetTicks();
            cout << "state = FROZEN" << endl;
            return true;
        }
    }
    return false;
}
