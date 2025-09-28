#ifndef BLOCKGRID_H
#define BLOCKGRID_H

#include "Coordinate.h"
#include <string>
#include <vector>

/**
 * @brief Controls solid blocks vs open passages
 */
class BlockGrid {
private:
    static const int MAP_ROWS = Coordinate::WORLD_ROWS;
    static const int MAP_COLS = Coordinate::WORLD_COLS;
    bool isBlocked[MAP_ROWS][MAP_COLS];

    std::vector<Coordinate> playerSpawns;
    std::vector<Coordinate> enemySpawns;
    std::vector<Coordinate> rockSpawns;

public:
    BlockGrid();
    bool isLocationBlocked(Coordinate spot) const;
    void clearPassageAt(Coordinate spot);
    bool isAreaBlocked(Coordinate topLeft, Coordinate bottomRight) const;
    void clearArea(Coordinate topLeft, Coordinate bottomRight);
    int countBlockedNeighbors(Coordinate center) const;
    void importMapFromFile(const std::string& filepath);
    void initializeDefaultMap();

    std::vector<Coordinate> getPlayerSpawns() const;
    std::vector<Coordinate> getEnemySpawns() const;
    std::vector<Coordinate> getRockSpawns() const;
    
    int getRows() const { return MAP_ROWS; }
    int getCols() const { return MAP_COLS; }
};

#endif // BLOCKGRID_H
