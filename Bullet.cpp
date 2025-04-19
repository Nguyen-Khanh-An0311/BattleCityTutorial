#include "Bullet.h"


Bullet::Bullet(int startX, int startY, int dirX, int dirY, SDL_Renderer* renderer) {
        x = startX;
        y = startY;
        dx = dirX;
        dy = dirY;

        // Gán góc xoay dựa trên hướng
        if (dx >0) angle = 0;
        else if (dx <0) angle = 180;
        else if (dy >0) angle = 90;
        else angle = 270;

        active = true;
        rect = {x, y, 20, 20};// Square shape bullet
        bulletTexture = IMG_LoadTexture(renderer, "bullet.png");
}

void Bullet::move() {
    x += dx;
    y += dy;
    rect = {x,y,10,10};

    if (x < TILE_SIZE || x > (MAP_WIDTH + 2) * TILE_SIZE ||
        y < TILE_SIZE || y > (MAP_HEIGHT + 1) * TILE_SIZE) {
        active = false;
    }
}

void Bullet::render(SDL_Renderer* renderer) {
    if (active) {
        SDL_RenderCopyEx(renderer, bulletTexture, nullptr, &rect, angle, nullptr, SDL_FLIP_NONE);
    }
}





