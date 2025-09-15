#ifndef BLOCKGRID_H
#define BLOCKGRID_H

#include "Coordinate.h"

class BlockGrid {
private:
    static const int MAP_ROWS = Coordinate::WORLD_ROWS;
    static const int MAP_COLS = Coordinate::WORLD_COLS;
    bool isBlocked[MAP_ROWS][MAP_COLS];

public:
    BlockGrid();
    bool isLocationBlocked(Coordinate spot) const;
    void clearPassageAt(Coordinate spot);
    void initializeDefaultMap();
    int getRows() const { return MAP_ROWS; }
    int getCols() const { return MAP_COLS; }
};

#endif
