#include "Coordinate.h"

Coordinate::Coordinate(int r, int c) : row(r), col(c) {
}

Coordinate Coordinate::operator+(const Coordinate& other) const {
    return Coordinate(row + other.row, col + other.col);
}

bool Coordinate::operator==(const Coordinate& other) const {
    return row == other.row && col == other.col;
}

bool Coordinate::isWithinBounds() const {
    return row >= 0 && row < WORLD_ROWS && col >= 0 && col < WORLD_COLS;
}
