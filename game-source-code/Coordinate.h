#ifndef COORDINATE_H
#define COORDINATE_H

#include <cmath>
#include <algorithm>

class Coordinate {
public:
    int row, col;
    static const int WORLD_ROWS = 20;
    static const int WORLD_COLS = 30;
    
    Coordinate(int r = 0, int c = 0);
    Coordinate operator+(const Coordinate& other) const;
    bool operator==(const Coordinate& other) const;
    bool operator!=(const Coordinate& other) const;
    bool isWithinBounds() const;
    Coordinate clampToBounds() const;
    float calculateDistance(const Coordinate& other) const;
    int manhattanDistance(const Coordinate& other) const;
};

#endif
