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
    
    // Add pre-made tunnels
    for (int col = 5; col < 20; ++col) {
        isBlocked[10][col] = false;
    }
    
    for (int row = 5; row < 15; ++row) {
        isBlocked[row][15] = false;
    }
    
    for (int col = 0; col < MAP_COLS; ++col) {
        isBlocked[0][col] = false;
        isBlocked[MAP_ROWS-1][col] = false;
    }
    for (int row = 0; row < MAP_ROWS; ++row) {
        isBlocked[row][0] = false;
        isBlocked[row][MAP_COLS-1] = false;
    }
}

bool BlockGrid::isAreaBlocked(Coordinate topLeft, Coordinate bottomRight) const {
    for (int row = topLeft.row; row <= bottomRight.row; ++row) {
        for (int col = topLeft.col; col <= bottomRight.col; ++col) {
            if (isLocationBlocked(Coordinate(row, col))) {
                return true;
            }
        }
    }
    return false;
}

void BlockGrid::clearArea(Coordinate topLeft, Coordinate bottomRight) {
    for (int row = topLeft.row; row <= bottomRight.row; ++row) {
        for (int col = topLeft.col; col <= bottomRight.col; ++col) {
            clearPassageAt(Coordinate(row, col));
        }
    }
}

int BlockGrid::countBlockedNeighbors(Coordinate center) const {
    int count = 0;
    
    for (int deltaRow = -1; deltaRow <= 1; ++deltaRow) {
        for (int deltaCol = -1; deltaCol <= 1; ++deltaCol) {
            if (deltaRow == 0 && deltaCol == 0) continue;
            
            Coordinate neighbor = center + Coordinate(deltaRow, deltaCol);
            if (isLocationBlocked(neighbor)) {
                count++;
            }
        }
    }
    
    return count;
}

void BlockGrid::importMapFromFile(const std::string& filepath) {
    initializeDefaultMap();
}
