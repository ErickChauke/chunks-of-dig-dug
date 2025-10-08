#ifndef BLOCKGRID_H
#define BLOCKGRID_H

#include "Coordinate.h"
#include <string>
#include <vector>

/**
 * @file BlockGrid.h
 * @brief Manages terrain state and tunnel system
 */

/**
 * @class BlockGrid
 * @brief Tracks which grid cells contain solid earth vs open passages
 * 
 * The BlockGrid maintains the game environment state, distinguishing between
 * solid earth blocks that must be tunneled through and open passages that
 * allow free movement. It also manages spawn points loaded from map files.
 * 
 * Grid dimensions: 20 rows × 30 columns
 * - Rows 0-2: HUD area (always clear)
 * - Rows 3-19: Playable area (earth and tunnels)
 * 
 * @note This is the single source of truth for terrain state
 */
class BlockGrid {
private:
    static const int MAP_ROWS = Coordinate::WORLD_ROWS;  ///< Total map rows
    static const int MAP_COLS = Coordinate::WORLD_COLS;  ///< Total map columns
    bool isBlocked[MAP_ROWS][MAP_COLS]; ///< Terrain state: true=solid, false=clear

    std::vector<Coordinate> playerSpawns;  ///< Player spawn positions from map
    std::vector<Coordinate> enemySpawns;   ///< Enemy spawn positions from map
    std::vector<Coordinate> rockSpawns;    ///< Rock spawn positions from map

public:
    /**
     * @brief Construct BlockGrid with default terrain layout
     * @note Initializes with procedural default map
     */
    BlockGrid();
    
    /**
     * @brief Check if location contains solid earth block
     * @param spot Coordinate to check
     * @return true if blocked by earth, false if clear passage
     * @note Returns true for out-of-bounds coordinates
     */
    bool isLocationBlocked(Coordinate spot) const;
    
    /**
     * @brief Clear earth block to create tunnel passage
     * @param spot Coordinate to clear
     * @note Called when player digs through earth
     */
    void clearPassageAt(Coordinate spot);
    
    /**
     * @brief Check if rectangular area contains any blocks
     * @param topLeft Top-left corner of area
     * @param bottomRight Bottom-right corner of area
     * @return true if any cell in area is blocked
     * @note Useful for object placement validation
     */
    bool isAreaBlocked(Coordinate topLeft, Coordinate bottomRight) const;
    
    /**
     * @brief Clear all blocks in rectangular area
     * @param topLeft Top-left corner of area
     * @param bottomRight Bottom-right corner of area
     * @note Creates chamber or passage in specified region
     */
    void clearArea(Coordinate topLeft, Coordinate bottomRight);
    
    /**
     * @brief Count blocked cells surrounding a position
     * @param center Center coordinate to check around
     * @return int Number of blocked neighbors (0-8)
     * @note Checks all 8 adjacent cells (including diagonals)
     */
    int countBlockedNeighbors(Coordinate center) const;
    
    /**
     * @brief Load terrain layout from map file
     * @param filepath Path to .txt map file
     * @note Falls back to default map if file not found
     * 
     * Map file format:
     * - '0' = clear passage
     * - '1' = earth block
     * - 'P' = player spawn
     * - 'E' = enemy spawn
     * - 'R' = rock spawn
     * - '#' = comment line
     */
    void importMapFromFile(const std::string& filepath);
    
    /**
     * @brief Initialize with procedural default terrain
     * @note Creates basic tunnel network with vertical and horizontal passages
     */
    void initializeDefaultMap();

    /**
     * @brief Get player spawn positions from loaded map
     * @return std::vector<Coordinate> List of player spawn points
     */
    std::vector<Coordinate> getPlayerSpawns() const;
    
    /**
     * @brief Get enemy spawn positions from loaded map
     * @return std::vector<Coordinate> List of enemy spawn points
     */
    std::vector<Coordinate> getEnemySpawns() const;
    
    /**
     * @brief Get rock spawn positions from loaded map
     * @return std::vector<Coordinate> List of rock spawn points
     */
    std::vector<Coordinate> getRockSpawns() const;
    
    /**
     * @brief Get number of rows in map
     * @return int Total rows (20)
     */
    int getRows() const { return MAP_ROWS; }
    
    /**
     * @brief Get number of columns in map
     * @return int Total columns (30)
     */
    int getCols() const { return MAP_COLS; }
};

#endif // BLOCKGRID_H
