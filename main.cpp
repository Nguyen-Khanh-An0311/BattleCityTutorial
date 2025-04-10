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
    cout << "=== Select Game Mode ===" << endl;
    cout << "1. Player vs Player" << endl;
    cout << "2. Coop vs Boss" << endl;
    cout << "Enter choice: ";
    int choice; cin >> choice;

    Game game;
    if(choice == 2) game.mode = GameMode::COOP_BOSS;
    else game.mode = GameMode::PVP;
    while(game.running){
        game.run();
    }
    return 0;
}
