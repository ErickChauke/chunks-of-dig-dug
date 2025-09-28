#ifndef COORDINATE_H
#define COORDINATE_H

/**
 * @brief Handles location and movement calculations
 * 
 * The Coordinate class represents a position in the game world using
 * row and column indices. Provides utilities for position arithmetic
 * and distance calculations.
 */
class Coordinate {
public:
    int row, col;
    
    // Total screen dimensions
    static const int WORLD_ROWS = 20;  // 800px / 40px = 20 rows
    static const int WORLD_COLS = 30;  // 1200px / 40px = 30 cols
    
    // Playable area (excluding HUD space)
    static const int HUD_ROWS = 3;     // Top 3 rows for HUD and sky
    static const int PLAYABLE_START_ROW = 3;  // Game starts at row 3
    static const int PLAYABLE_ROWS = WORLD_ROWS - HUD_ROWS; // 17 playable rows
    
    /**
     * @brief Construct a new Coordinate object
     * @param r Row position (default: 0)
     * @param c Column position (default: 0)
     */
    Coordinate(int r = 0, int c = 0);
    
    /**
     * @brief Add two coordinates together
     * @param other The coordinate to add
     * @return Coordinate Result of addition
     */
    Coordinate operator+(const Coordinate& other) const;
    
    /**
     * @brief Check equality with another coordinate
     * @param other The coordinate to compare
     * @return true if equal, false otherwise
     */
    bool operator==(const Coordinate& other) const;
    
    /**
     * @brief Check inequality with another coordinate
     * @param other The coordinate to compare
     * @return true if not equal, false otherwise
     */
    bool operator!=(const Coordinate& other) const;
    
    /**
     * @brief Check if coordinate is within valid game bounds (playable area)
     * @return true if within bounds, false otherwise
     */
    bool isWithinBounds() const;
    
    /**
     * @brief Check if coordinate is within playable game area
     * @return true if in playable area, false if in HUD area
     */
    bool isInPlayableArea() const;
    
    /**
     * @brief Clamp coordinate to valid bounds
     * @return Coordinate Clamped coordinate within bounds
     */
    Coordinate clampToBounds() const;
    
    /**
     * @brief Calculate Euclidean distance to another coordinate
     * @param other Target coordinate
     * @return float Distance between coordinates
     */
    float calculateDistance(const Coordinate& other) const;
    
    /**
     * @brief Calculate Manhattan distance to another coordinate
     * @param other Target coordinate
     * @return int Manhattan distance (sum of row and column differences)
     */
    int manhattanDistance(const Coordinate& other) const;
};

#endif // COORDINATE_H
