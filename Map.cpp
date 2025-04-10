#include "Map.h"

void Map::loadFromFile(const string& filename, SDL_Renderer* renderer) {
    walls.clear();

    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Cannot open map file: " << filename << endl;
        return;
    }

    string line;
    int row = 0;
    while (getline(file, line) && row < MAP_HEIGHT) {
        for (int col = 0; col < line.length() && col < MAP_WIDTH; ++col) {
            if (line[col] == '1') {
                Wall wall(col * TILE_SIZE, row * TILE_SIZE, renderer);
                walls.push_back(wall);
            }
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
