#include "Map.h"

void Map::loadFromFile(const string& filename, SDL_Renderer* renderer) {
    walls.clear();

    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Cannot open map file: " << filename << endl;
        return;
    }

    string line;
    int row = 1;
    while (getline(file, line) && row <= MAP_HEIGHT) {
        for (int col = 1; col < line.length(); ++col) {
            if (line[col] == '#') {
                Wall wall(col * TILE_SIZE, row * TILE_SIZE, renderer);
                walls.push_back(wall);
            }
            else if (line[col] == '~') {
                Water water(col * TILE_SIZE, row * TILE_SIZE, renderer);
                waters.push_back(water);
            }
            else if (line[col] == '%'){
                Bush bush(col * TILE_SIZE, row * TILE_SIZE, renderer);
                bushs.push_back(bush);
            }
            else if (line[col] == '@'){
                Stone stone(col * TILE_SIZE, row * TILE_SIZE, renderer);
                stones.push_back(stone);
            }
            else if (line[col] == '-'){
                Ice ice(col * TILE_SIZE, row * TILE_SIZE, renderer);
                ices.push_back(ice);
            }
            else continue;
        }
        ++row;
    }

    file.close();
}

void Map::generateRandom(SDL_Renderer* renderer) {
    walls.clear();  // Xóa toàn bộ tường cũ nếu có
    srand(static_cast<unsigned int>(time(nullptr))); // Khởi tạo seed cho rand

    for (int y = 0; y < MAP_HEIGHT; ++y) {
        for (int x = 0; x < MAP_WIDTH; ++x) {
            if (rand() % 100 < 30) { // 30% khả năng tạo tường tại mỗi ô
                Wall wall(x * TILE_SIZE, y * TILE_SIZE, renderer); // Tạo tường
                walls.push_back(wall); // Thêm vào danh sách
            }
        }
    }
}


vector<Wall>& Map::getWalls(){
    return walls;
}
vector<Water>& Map::getWaters(){
    return waters;
}
