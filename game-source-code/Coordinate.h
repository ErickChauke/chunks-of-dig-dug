#ifndef COORDINATE_H
#define COORDINATE_H

/**
 * @brief Handles location and movement calculations
 */
class Coordinate {
public:
    int row, col;
    
    static const int WORLD_ROWS = 20;
    static const int WORLD_COLS = 30;
    
    /**
     * @brief Construct a new Coordinate object
     */
    Coordinate(int r = 0, int c = 0);
    
    /**
     * @brief Add two coordinates together
     */
    Coordinate operator+(const Coordinate& other) const;
    
    /**
     * @brief Check equality with another coordinate
     */
    bool operator==(const Coordinate& other) const;
    
    /**
     * @brief Check if coordinate is within valid game bounds
     */
    bool isWithinBounds() const;
};

#endif // COORDINATE_H
