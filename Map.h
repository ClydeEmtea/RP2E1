#ifndef MAP_CLASS_H
#define MAP_CLASS_H

#include <iostream>
#include <algorithm>
#include <random>
#include <vector>

class Map {
public:
    Map(int l, int w);
    ~Map();  // Destructor to free allocated memory

    int** map;

private:
    void initMap();
    void createMaze(int x, int y);
    void shuffleArray(int** directions);
    void setFinish();

    int length, width;
};
#endif
