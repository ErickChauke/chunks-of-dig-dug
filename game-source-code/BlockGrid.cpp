#include "BlockGrid.h"
#include <fstream>
#include <iostream>

BlockGrid::BlockGrid() {
    initializeDefaultMap();
}

bool BlockGrid::isLocationBlocked(Coordinate spot) const {
    if (!spot.isInPlayableArea()) {
        return true; // Treat out-of-playable-area as blocked
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
        std::cout << "Warning: Could not open map file " << filepath 
                  << ", using default map" << std::endl;
        initializeDefaultMap();
        return;
    }
    
    std::cout << "Loading map from: " << filepath << std::endl;
    
    // Clear previous spawn data
    playerSpawns.clear();
    enemySpawns.clear();
    rockSpawns.clear();
    
    std::string line;
    int row = 0;
    
    while (std::getline(file, line) && row < MAP_ROWS) {
        // Skip comment lines and empty lines
        if (line.empty() || line[0] == '#') {
            continue;
        }
        
        // Process each character in the line
        for (int col = 0; col < MAP_COLS && col < (int)line.length(); ++col) {
            char cell = line[col];
            Coordinate pos(row, col);
            
            switch (cell) {
                case '0': // Clear passage
                    isBlocked[row][col] = false;
                    break;
                case '1': // Earth block
                    isBlocked[row][col] = true;
                    break;
                case 'P': // Player spawn
                    isBlocked[row][col] = false;
                    playerSpawns.push_back(pos);
                    break;
                case 'E': // Enemy spawn
                    isBlocked[row][col] = false;
                    enemySpawns.push_back(pos);
                    break;
                case 'R': // Rock spawn
                    isBlocked[row][col] = false;
                    rockSpawns.push_back(pos);
                    break;
                default:
                    // Unknown character, treat as earth
                    isBlocked[row][col] = true;
                    break;
            }
        }
        row++;
    }
    
    file.close();
}

void BlockGrid::initializeDefaultMap() {
    // Initialize all positions based on playable area
    for (int row = 0; row < MAP_ROWS; ++row) {
        for (int col = 0; col < MAP_COLS; ++col) {
            if (row < Coordinate::PLAYABLE_START_ROW) {
                isBlocked[row][col] = false; // HUD area
            } else {
                isBlocked[row][col] = true;  // Earth by default
            }
        }
    }
    
    // Create spawn area at top-left of playable area
    for (int row = Coordinate::PLAYABLE_START_ROW; row < Coordinate::PLAYABLE_START_ROW + 3; ++row) {
        for (int col = 0; col < 5; ++col) {
            isBlocked[row][col] = false;
        }
    }
    
    // Create some pre-made tunnels
    int midRow = Coordinate::PLAYABLE_START_ROW + (Coordinate::PLAYABLE_ROWS / 2);
    for (int col = 5; col < 20; ++col) {
        isBlocked[midRow][col] = false;
    }
    
    // Create border passages
    for (int row = Coordinate::PLAYABLE_START_ROW; row < Coordinate::WORLD_ROWS; ++row) {
        isBlocked[row][0] = false;
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
