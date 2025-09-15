#include "BlockGrid.h"

BlockGrid::BlockGrid() {
    initializeDefaultMap();
}

bool BlockGrid::isLocationBlocked(Coordinate spot) const {
    if (!spot.isWithinBounds()) {
        return true;
    }
    return isBlocked[spot.row][spot.col];
}

void BlockGrid::clearPassageAt(Coordinate spot) {
    if (spot.isWithinBounds()) {
        isBlocked[spot.row][spot.col] = false;
    }
}

void BlockGrid::initializeDefaultMap() {
    for (int row = 0; row < MAP_ROWS; ++row) {
        for (int col = 0; col < MAP_COLS; ++col) {
            isBlocked[row][col] = true;
        }
    }
    
    for (int row = 0; row < 3; ++row) {
        for (int col = 0; col < 5; ++col) {
            isBlocked[row][col] = false;
        }
    }
}
