#include<iostream>
#include<SDL.h>
#include "Game.h"


using namespace std;



int main(int argc, char* argv[]){
    Game game;
    while(game.running){
        game.run();
    }
    return 0;
}
