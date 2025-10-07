#include "BlockGrid.h"
#include <fstream>
#include <iostream>

BlockGrid::BlockGrid() {
    initializeDefaultMap();
}

bool BlockGrid::isLocationBlocked(Coordinate spot) const {
    if (!spot.isInPlayableArea()) {
        return true;
    }
    return isBlocked[spot.row][spot.col];
}

void BlockGrid::clearPassageAt(Coordinate spot) {
    if (spot.isInPlayableArea()) {
        isBlocked[spot.row][spot.col] = false;
    }
}

void BlockGrid::importMapFromFile(const std::string& filepath) {
    std::ifstream file(filepath);
    if (!file.is_open()) {
        std::cout << "Map file not found: " << filepath << std::endl;
        std::cout << "Using procedural default map instead" << std::endl;
        initializeDefaultMap();
        return;
    }
    
    std::cout << "Loading map from: " << filepath << std::endl;
    
    playerSpawns.clear();
    enemySpawns.clear();
    rockSpawns.clear();
    
    std::string line;
    int row = 0;
    
    while (std::getline(file, line) && row < MAP_ROWS) {
        if (line.empty() || line[0] == '#') {
            continue;
        }
        
        for (int col = 0; col < MAP_COLS && col < (int)line.length(); ++col) {
            char cell = line[col];
            Coordinate pos(row, col);
            
            switch (cell) {
                case '0':
                    isBlocked[row][col] = false;
                    break;
                case '1':
                    isBlocked[row][col] = true;
                    break;
                case 'P':
                    isBlocked[row][col] = false;
                    playerSpawns.push_back(pos);
                    break;
                case 'E':
                    isBlocked[row][col] = false;
                    enemySpawns.push_back(pos);
                    break;
                case 'R':
                    isBlocked[row][col] = false;
                    rockSpawns.push_back(pos);
                    break;
                default:
                    isBlocked[row][col] = true;
                    break;
            }
        }
        row++;
    }
    
    file.close();
    
    if (playerSpawns.empty() && enemySpawns.empty() && rockSpawns.empty()) {
        std::cout << "Map file contained no spawn data, using defaults" << std::endl;
        initializeDefaultMap();
    } else {
        std::cout << "Map loaded successfully" << std::endl;
    }
}

void BlockGrid::initializeDefaultMap() {
    for (int row = 0; row < MAP_ROWS; ++row) {
        for (int col = 0; col < MAP_COLS; ++col) {
            if (row < Coordinate::PLAYABLE_START_ROW) {
                isBlocked[row][col] = false;
            } else {
                isBlocked[row][col] = true;
            }
        }
    }
    
    for (int row = Coordinate::PLAYABLE_START_ROW; row < Coordinate::PLAYABLE_START_ROW + 3; ++row) {
        for (int col = 0; col < 5; ++col) {
            isBlocked[row][col] = false;
        }
    }
    
    int midRow = Coordinate::PLAYABLE_START_ROW + (Coordinate::PLAYABLE_ROWS / 2);
    
    for (int col = 5; col < 20; ++col) {
        isBlocked[midRow][col] = false;
    }
    
    for (int row = Coordinate::PLAYABLE_START_ROW + 2; row < Coordinate::PLAYABLE_START_ROW + 12; ++row) {
        isBlocked[row][15] = false;
    }
    
    for (int row = Coordinate::WORLD_ROWS - 3; row < Coordinate::WORLD_ROWS; ++row) {
        for (int col = 25; col < 28; ++col) {
            isBlocked[row][col] = false;
        }
    }
    
    for (int col = 20; col < 25; ++col) {
        isBlocked[Coordinate::WORLD_ROWS - 2][col] = false;
    }
    
    for (int row = Coordinate::PLAYABLE_START_ROW; row < Coordinate::WORLD_ROWS; ++row) {
        isBlocked[row][0] = false;
    }
    for (int row = Coordinate::PLAYABLE_START_ROW; row < Coordinate::WORLD_ROWS; ++row) {
        isBlocked[row][Coordinate::WORLD_COLS-1] = false;
    }
    for (int col = 0; col < Coordinate::WORLD_COLS; ++col) {
        isBlocked[Coordinate::WORLD_ROWS-1][col] = false;
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

std::vector<Coordinate> BlockGrid::getPlayerSpawns() const {
    return playerSpawns;
}

std::vector<Coordinate> BlockGrid::getEnemySpawns() const {
    return enemySpawns;
}

std::vector<Coordinate> BlockGrid::getRockSpawns() const {
    return rockSpawns;
}
