#ifndef COORDINATE_H
#define COORDINATE_H

/**
 * @file Coordinate.h
 * @brief Grid-based position system for game world
 * 
 * Provides discrete coordinate system with row/column positioning,
 * boundary checking, and distance calculations for the 20x30 game grid.
 */

/**
 * @class Coordinate
 * @brief Represents a position in the game world using grid coordinates
 * 
 * The Coordinate class uses a discrete grid system where each position
 * is defined by integer row and column values. The game world is divided
 * into a HUD area (rows 0-2) and a playable area (rows 3-19).
 * 
 * World dimensions: 20 rows × 30 columns
 * Cell size: 40×40 pixels
 * 
 * @note Position (0,0) is top-left corner
 */
class Coordinate {
public:
    int row; ///< Vertical position (0-19)
    int col; ///< Horizontal position (0-29)
    
    static const int WORLD_ROWS = 20;  ///< Total screen rows (800px / 40px)
    static const int WORLD_COLS = 30;  ///< Total screen columns (1200px / 40px)
    static const int HUD_ROWS = 3;     ///< Top rows reserved for HUD
    static const int PLAYABLE_START_ROW = 3;  ///< First playable row
    static const int PLAYABLE_ROWS = WORLD_ROWS - HUD_ROWS; ///< Available play area rows
    
    /**
     * @brief Construct coordinate at specified position
     * @param r Row position (default: 0)
     * @param c Column position (default: 0)
     */
    Coordinate(int r = 0, int c = 0);
    
    /**
     * @brief Add two coordinates (vector addition)
     * @param other Coordinate to add
     * @return Coordinate Result of addition
     * @note Useful for applying movement offsets
     */
    Coordinate operator+(const Coordinate& other) const;
    
    /**
     * @brief Check equality with another coordinate
     * @param other Coordinate to compare
     * @return true if positions match exactly
     */
    bool operator==(const Coordinate& other) const;
    
    /**
     * @brief Check inequality with another coordinate
     * @param other Coordinate to compare
     * @return true if positions differ
     */
    bool operator!=(const Coordinate& other) const;
    
    /**
     * @brief Verify coordinate is within valid game bounds
     * @return true if within playable area (rows 3-19, cols 0-29)
     * @note HUD area (rows 0-2) is not considered within bounds
     */
    bool isWithinBounds() const;
    
    /**
     * @brief Check if coordinate is in playable game area
     * @return true if in playable area (not HUD)
     */
    bool isInPlayableArea() const;
    
    /**
     * @brief Constrain coordinate to valid bounds
     * @return Coordinate Clamped to nearest valid position
     * @note Returns position within playable area if out of bounds
     */
    Coordinate clampToBounds() const;
    
    /**
     * @brief Calculate Euclidean distance to another coordinate
     * @param other Target coordinate
     * @return float Distance in grid cells
     * @note Uses sqrt((Δrow)² + (Δcol)²)
     */
    float calculateDistance(const Coordinate& other) const;
    
    /**
     * @brief Calculate Manhattan distance (grid movement distance)
     * @param other Target coordinate
     * @return int Manhattan distance |Δrow| + |Δcol|
     * @note Represents minimum moves needed in 4-directional movement
     */
    int manhattanDistance(const Coordinate& other) const;
};

#endif // COORDINATE_H
