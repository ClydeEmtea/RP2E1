#include "Map.h"

Map::Map(int l, int w) : length(l), width(w) {
    initMap();
    createMaze(0, 0);
}

Map::~Map() {
    for (int i = 0; i < length; ++i) {
        delete[] map[i];
    }
    delete[] map;
}

void Map::initMap() {
    map = new int* [length];
    for (int i = 0; i < length; ++i) {
        map[i] = new int[width];
        std::fill(map[i], map[i] + width, 1);
    }
}

void Map::createMaze(int x, int y) {
    map[y][x] = 0;

    int* directions[] = { new int[2] {1, 0}, new int[2] {-1, 0}, new int[2] {0, 1}, new int[2] {0, -1} };
    shuffleArray(directions);

    for (const auto& direction : directions) {
        int dx = direction[0];
        int dy = direction[1];
        int nx = x + dx * 2;
        int ny = y + dy * 2;

        if (0 <= nx && nx < width && 0 <= ny && ny < length && map[ny][nx] == 1) {
            map[ny - dy][nx - dx] = 0;
            createMaze(nx, ny);
        }
    }

    for (int i = 0; i < 4; ++i) {
        delete[] directions[i];
    }
}

void Map::shuffleArray(int** directions) {
    std::random_device rd;
    std::mt19937 g(rd());

    for (int i = 3; i > 0; --i) {
        int index = std::uniform_int_distribution<>(0, i)(g);
        std::swap(directions[index], directions[i]);
    }
}
