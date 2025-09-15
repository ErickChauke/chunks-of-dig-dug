#include <iostream>
#include "Coordinate.h"

int main() {
    Coordinate pos(1, 1);
    std::cout << "Underground Adventure - Coordinate System Initialized" << std::endl;
    std::cout << "Position: (" << pos.row << ", " << pos.col << ")" << std::endl;
    std::cout << "World size: " << Coordinate::WORLD_ROWS << "x" << Coordinate::WORLD_COLS << std::endl;
    return 0;
}
