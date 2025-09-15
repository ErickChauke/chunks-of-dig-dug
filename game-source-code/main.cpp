#include <iostream>
#include "Coordinate.h"
#include "BlockGrid.h"
#include "Player.h"

int main() {
    std::cout << "Underground Adventure - Player Test" << std::endl;
    
    BlockGrid terrain;
    Player player(Coordinate(1, 1));
    
    std::cout << "Player at: (" << player.getPosition().row 
              << ", " << player.getPosition().col << ")" << std::endl;
    
    bool moved = player.moveInDirection(Direction::RIGHT, terrain);
    std::cout << "Moved right: " << (moved ? "Success" : "Failed") << std::endl;
    std::cout << "New position: (" << player.getPosition().row 
              << ", " << player.getPosition().col << ")" << std::endl;
    
    moved = player.moveInDirection(Direction::DOWN, terrain);
    std::cout << "Dug down: " << (moved ? "Success" : "Failed") << std::endl;
    std::cout << "Tunnels: " << player.getTunnelsCreated() << std::endl;
    
    return 0;
}
