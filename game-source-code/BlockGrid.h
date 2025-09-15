#ifndef BLOCKGRID_H
#define BLOCKGRID_H

#include "Coordinate.h"
#include <string>

class BlockGrid {
private:
    static const int MAP_ROWS = Coordinate::WORLD_ROWS;
    static const int MAP_COLS = Coordinate::WORLD_COLS;
    bool isBlocked[MAP_ROWS][MAP_COLS];

public:
    BlockGrid();
    bool isLocationBlocked(Coordinate spot) const;
    void clearPassageAt(Coordinate spot);
    bool isAreaBlocked(Coordinate topLeft, Coordinate bottomRight) const;
    void clearArea(Coordinate topLeft, Coordinate bottomRight);
    int countBlockedNeighbors(Coordinate center) const;
    void importMapFromFile(const std::string& filepath);
    void initializeDefaultMap();
    int getRows() const { return MAP_ROWS; }
    int getCols() const { return MAP_COLS; }
};

#endif
