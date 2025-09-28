#ifndef BLOCKGRID_H
#define BLOCKGRID_H

#include "Coordinate.h"
#include <string>
#include <vector>

/**
 * @brief Controls solid blocks vs open passages
 * 
 * The BlockGrid class manages the game environment, tracking which
 * locations contain solid earth blocks and which are open passages
 * that the player and enemies can move through.
 */
class BlockGrid {
private:
    // Use the same dimensions as Coordinate class
    static const int MAP_ROWS = Coordinate::WORLD_ROWS;  // 20 rows
    static const int MAP_COLS = Coordinate::WORLD_COLS;  // 30 cols
    bool isBlocked[MAP_ROWS][MAP_COLS];

    std::vector<Coordinate> playerSpawns;
    std::vector<Coordinate> enemySpawns;
    std::vector<Coordinate> rockSpawns;

public:
    /**
     * @brief Construct a new BlockGrid object
     */
    BlockGrid();
    
    /**
     * @brief Check if a location contains a solid block
     * @param spot Coordinate to check
     * @return true if blocked, false if open passage
     */
    bool isLocationBlocked(Coordinate spot) const;
    
    /**
     * @brief Clear a block to create an open passage
     * @param spot Coordinate to clear
     */
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
