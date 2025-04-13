#include<iostream>
#include<SDL.h>
#include "Game.h"
#include "Statistics.h"

const int TILE_SIZE = 40;
const int SCREEN_WIDTH = TILE_SIZE*35;
const int SCREEN_HEIGHT = TILE_SIZE*28;
const int MAP_WIDTH = 25;
const int MAP_HEIGHT = 26;


using namespace std;


int main(int argc, char* argv[]){
    Game game;
    while (game.running){
        game.run();
    }
    return 0;
}
