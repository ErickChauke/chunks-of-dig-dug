#include "Coordinate.h"

Coordinate::Coordinate(int r, int c) : row(r), col(c) {
}

Coordinate Coordinate::operator+(const Coordinate& other) const {
    return Coordinate(row + other.row, col + other.col);
}

bool Coordinate::operator==(const Coordinate& other) const {
    return row == other.row && col == other.col;
}

bool Coordinate::operator!=(const Coordinate& other) const {
    return !(*this == other);
}

bool Coordinate::isWithinBounds() const {
    return row >= 0 && row < WORLD_ROWS && col >= 0 && col < WORLD_COLS;
}

Coordinate Coordinate::clampToBounds() const {
    return Coordinate(
        std::max(0, std::min(row, WORLD_ROWS - 1)),
        std::max(0, std::min(col, WORLD_COLS - 1))
    );
}

float Coordinate::calculateDistance(const Coordinate& other) const {
    int deltaRow = row - other.row;
    int deltaCol = col - other.col;
    return std::sqrt(static_cast<float>(deltaRow * deltaRow + deltaCol * deltaCol));
}

int Coordinate::manhattanDistance(const Coordinate& other) const {
    return std::abs(row - other.row) + std::abs(col - other.col);
}
