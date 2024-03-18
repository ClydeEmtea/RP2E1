#include "Map.h"

Map::Map(int l, int w) : length(l), width(w) {
    initMap();
    createMaze(0, 0);
    for (int i = 0; i < length-1; ++i)
    {
        for (int j = 0; j < width-1; ++j)
        {
            if (rand() % 5 == 2)
            {
                map[i][j] = 0;
            }
            if (i <= 3 && j <= 3)
            {
            	map[i][j] = 0;
			}
        }
    }
    setFinish();
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

void Map::setFinish() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, length - 1);
    int finish_x = dis(gen);
    int finish_y = dis(gen);
    while (finish_x < 3 || finish_y < 3 || finish_x > length - 3 || finish_y > width - 3)
	{
		finish_x = dis(gen);
    	finish_y = dis(gen);
	}


    // Set start and finish positions

    for (int i = finish_y - 1; i <= finish_y + 1; ++i) {
        for (int j = finish_x - 1; j <= finish_x + 1; ++j) {
            if (i >= 0 && i < length && j >= 0 && j < width) {
                map[i][j] = 0;
            }
        }
    }
    map[finish_y][finish_x] = 2;  // Finish position

}