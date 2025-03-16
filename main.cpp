#include<iostream>
#include<SDL.h>


using namespace std;

class Game;

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const int TILE_SIZE = 40;
const int MAP_WIDTH = SCREEN_WIDTH / TILE_SIZE;
const int MAP_HEIGHT = SCREEN_HEIGHT / TILE_SIZE;

class Game{
    public:
        SDL_Window* window;
        SDL_Renderer* renderer;
        bool running;

        Game(){
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
                SDL_WINDOW_SHOWN | SDL_WINDOW_FULLSCREEN
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
        }
        void render(){
            SDL_SetRenderDrawColor(renderer, 128, 128, 128, 255); //chọn màu
            SDL_RenderClear(renderer); // tô toàn bộ màu vừa chọn

            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // chọn màu mới
            for(int i=1; i < MAP_HEIGHT - 1; i++){
                for(int j=1; j < MAP_WIDTH - 1; j++){
                    SDL_Rect tile = {j*TILE_SIZE, i*TILE_SIZE, TILE_SIZE, TILE_SIZE}; // tạo hcn tên tile (x,y,cr,cd)
                    SDL_RenderFillRect(renderer, &tile);
                }
            }
            SDL_RenderPresent(renderer); // hiển thị tất cả, phải có

        }
        void run() {
            if(running){
                render();
                SDL_Delay(16);
            }
        }
        ~Game(){
            SDL_DestroyRenderer(renderer);
            SDL_DestroyWindow(window);
            SDL_Quit();
        }

};


int main(int argc, char* argv[]){
    Game game;
    while(game.running){
        game.run();
    }
    return 0;
}
