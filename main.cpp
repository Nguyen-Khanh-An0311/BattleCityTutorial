#include<iostream>
#include<SDL.h>
#include "Game.h"
#include "Statistics.h"

const int SCREEN_WIDTH = 1500;
const int SCREEN_HEIGHT = 1000;
const int TILE_SIZE = 100;
const int MAP_WIDTH = SCREEN_WIDTH / TILE_SIZE;
const int MAP_HEIGHT = SCREEN_HEIGHT / TILE_SIZE;


using namespace std;



int main(int argc, char* argv[]){
    Game game;
    while(game.running){
        game.run();
    }
    return 0;
}
