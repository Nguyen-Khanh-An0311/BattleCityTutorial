#ifndef MAP_H_INCLUDED
#define MAP_H_INCLUDED

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <ctime>
#include "Wall.h"         // dùng class Wall bạn đã tạo
#include <SDL.h>
#include "Statistics.h"
#include "Water.h"
#include "Bush.h"
#include "Stone.h"
#include "Ice.h"

using namespace std;


class Map {
public:
    vector<Wall> walls;
    vector<Water> waters;
    vector<Bush> bushs;
    vector<Stone> stones;
    vector<Ice> ices;
    void loadFromFile(const string& filename, SDL_Renderer* renderer);
    void generateRandom(SDL_Renderer* renderer);
    void render(SDL_Renderer* renderer);
    vector<Wall>& getWalls();
    vector<Water>& getWaters();
};

#endif // MAP_H_INCLUDED
