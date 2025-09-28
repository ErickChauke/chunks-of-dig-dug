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
    
    Coordinate(int r = 0, int c = 0);
    Coordinate operator+(const Coordinate& other) const;
    bool operator==(const Coordinate& other) const;
    bool operator!=(const Coordinate& other) const;
    bool isWithinBounds() const;
    bool isInPlayableArea() const;
    Coordinate clampToBounds() const;
    float calculateDistance(const Coordinate& other) const;
    int manhattanDistance(const Coordinate& other) const;
};

#endif // COORDINATE_H
