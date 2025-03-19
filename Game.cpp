#include "Game.h"
#include <iostream>
#include <SDL.h>
using namespace std;

Game::Game(){
            running = true;
            if(SDL_Init(SDL_INIT_EVERYTHING) < 0){
                cout << "SDL could not initialize! SDL_Error: " << SDL_GetError() << endl;
                running = false;
            }

            window = SDL_CreateWindow(
                "Battle City",
                SDL_WINDOWPOS_CENTERED,
                SDL_WINDOWPOS_CENTERED,
                SCREEN_WIDTH,
                SCREEN_HEIGHT,
                SDL_WINDOW_SHOWN
            );
            if(!window){
                cout << "Window could not be created! SDL_Error: " << SDL_GetError() << endl;
                running = false;
            }

            renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
            if(!renderer){
                cout << "Renderer could not be created! SDL_Erroe: " << SDL_GetError() << endl;
                running = false;
            }
            generateWalls();
        }
void Game::render(){
            SDL_SetRenderDrawColor(renderer, 128, 128, 128, 255); //chọn màu
            SDL_RenderClear(renderer); // tô toàn bộ màu vừa chọn

            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // chọn màu mới
            for(int i=1; i < MAP_HEIGHT - 1; i++){
                for(int j=1; j < MAP_WIDTH - 1; j++){
                    SDL_Rect tile = {j*TILE_SIZE, i*TILE_SIZE, TILE_SIZE, TILE_SIZE}; // tạo hcn tên tile (x,y,cr,cd)
                    SDL_RenderFillRect(renderer, &tile);
                }
            }

            for(int i=0; i<walls.size(); i++){
                walls[i].render(renderer);
            }

            SDL_RenderPresent(renderer); // hiển thị tất cả, phải có
        }
void Game::run() {
            if(running){
                render();
                SDL_Delay(16);
            }
        }
Game::~Game(){
            SDL_DestroyRenderer(renderer);
            SDL_DestroyWindow(window);
            SDL_Quit();
        }

void Game::generateWalls(){
    for (int i = 3; i < MAP_HEIGHT - 3; i += 2) {
        for (int j = 3; j < MAP_WIDTH - 3; j += 2) {
            Wall w = Wall(j * TILE_SIZE, i * TILE_SIZE);
            walls.push_back(w);
        }
    }
}

