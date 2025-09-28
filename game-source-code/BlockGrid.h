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
    
    /**
     * @brief Check if an area contains any blocks
     * @param topLeft Top-left corner of area
     * @param bottomRight Bottom-right corner of area
     * @return true if any blocks in area, false if all clear
     */
    bool isAreaBlocked(Coordinate topLeft, Coordinate bottomRight) const;
    
    /**
     * @brief Clear all blocks in an area
     * @param topLeft Top-left corner of area
     * @param bottomRight Bottom-right corner of area
     */
    void clearArea(Coordinate topLeft, Coordinate bottomRight);
    
    /**
     * @brief Count blocked neighbors around a coordinate
     * @param center Center coordinate to check around
     * @return int Number of blocked neighbors (0-8)
     */
    int countBlockedNeighbors(Coordinate center) const;
    
    /**
     * @brief Load map layout from file
     * @param filepath Path to map file
     */
    void importMapFromFile(const std::string& filepath);
    
    /**
     * @brief Initialize with default solid earth layout for smaller world
     */
    void initializeDefaultMap();

    /**
     * @brief Get spawn positions from loaded map
     */
    std::vector<Coordinate> getPlayerSpawns() const;
    std::vector<Coordinate> getEnemySpawns() const;
    std::vector<Coordinate> getRockSpawns() const;
    
    /**
     * @brief Get world dimensions
     */
    int getRows() const { return MAP_ROWS; }
    int getCols() const { return MAP_COLS; }
};

#endif // BLOCKGRID_H
