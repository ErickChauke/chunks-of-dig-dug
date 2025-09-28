#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include "../game-source-code/Coordinate.h"
#include "../game-source-code/BlockGrid.h"
#include "../game-source-code/Player.h"
#include "../game-source-code/Enemy.h"
#include "../game-source-code/EnemyLogic.h"
#include "../game-source-code/Rock.h"
#include "../game-source-code/Harpoon.h"

TEST_CASE("Coordinate System") {
    SUBCASE("Constructor and member access") {
        Coordinate coord(5, 10);
        CHECK(coord.row == 5);
        CHECK(coord.col == 10);
    }
    
    SUBCASE("Default constructor") {
        Coordinate coord;
        CHECK(coord.row == 0);
        CHECK(coord.col == 0);
    }
    
    SUBCASE("Addition operator") {
        Coordinate coord1(3, 4);
        Coordinate coord2(1, 2);
        Coordinate result = coord1 + coord2;
        CHECK(result.row == 4);
        CHECK(result.col == 6);
    }
    
    SUBCASE("Distance calculation") {
        Coordinate coord1(0, 0);
        Coordinate coord2(3, 4);
        float distance = coord1.calculateDistance(coord2);
        CHECK(distance == doctest::Approx(5.0f));
    }
}

TEST_CASE("Screen Boundary Restrictions") {
    SUBCASE("World dimensions") {
        CHECK(Coordinate::WORLD_ROWS == 20);
        CHECK(Coordinate::WORLD_COLS == 30);
    }
    
    SUBCASE("Playable coordinates only") {
        Coordinate validCoord(10, 15);
        CHECK(validCoord.isWithinBounds() == true);
        
        Coordinate playableCoord(5, 5);
        CHECK(playableCoord.isWithinBounds() == true);
    }
    
    SUBCASE("Invalid coordinates") {
        Coordinate invalidRow(-1, 5);
        Coordinate invalidCol(5, -1);
        CHECK(invalidRow.isWithinBounds() == false);
        CHECK(invalidCol.isWithinBounds() == false);
        
        Coordinate tooLargeRow(20, 15);
        Coordinate tooLargeCol(15, 30);
        CHECK(tooLargeRow.isWithinBounds() == false);
        CHECK(tooLargeCol.isWithinBounds() == false);
    }
}

TEST_CASE("Terrain Management") {
    BlockGrid terrain;
    
    SUBCASE("Tunnel digging functionality") {
        Coordinate digPos(8, 8);
        
        terrain.clearPassageAt(digPos);
        CHECK(terrain.isLocationBlocked(digPos) == false);
    }
}

TEST_CASE("Player Movement") {
    SUBCASE("Initial state") {
        Player player(Coordinate(1, 1));
        CHECK(player.getPosition().row == 1);
        CHECK(player.getPosition().col == 1);
        CHECK(player.isActive() == true);
        CHECK(player.getTunnelsCreated() == 0);
    }
    
    SUBCASE("Tunnel digging") {
        Player player(Coordinate(5, 5));
        BlockGrid terrain;
        int initialTunnels = player.getTunnelsCreated();
        
        Coordinate earthPos(5, 6);
        CHECK(terrain.isLocationBlocked(earthPos) == true);
        
        bool dugTunnel = player.moveInDirection(Direction::RIGHT, terrain);
        CHECK(dugTunnel == true);
        CHECK(player.getPosition().col == 6);
        CHECK(terrain.isLocationBlocked(earthPos) == false);
        CHECK(player.getTunnelsCreated() == initialTunnels + 1);
    }
}

TEST_CASE("Enemy AI System") {
    SUBCASE("Enemy initialization") {
        Enemy enemy(Coordinate(10, 10), EnemyType::RED_MONSTER);
        CHECK(enemy.getPosition().row == 10);
        CHECK(enemy.getPosition().col == 10);
        CHECK(enemy.isActive() == true);
        CHECK(enemy.getEnemyType() == EnemyType::RED_MONSTER);
        CHECK(enemy.getHealth() == 1);
    }
    
    SUBCASE("Enemy types and health") {
        Enemy redEnemy(Coordinate(5, 5), EnemyType::RED_MONSTER);
        Enemy aggressiveEnemy(Coordinate(5, 6), EnemyType::AGGRESSIVE_MONSTER);
        Enemy fastEnemy(Coordinate(5, 7), EnemyType::FAST_MONSTER);
        
        CHECK(redEnemy.getHealth() == 1);
        CHECK(aggressiveEnemy.getHealth() == 2);
        CHECK(fastEnemy.getHealth() == 1);
    }
    
    SUBCASE("Enemy damage system") {
        Enemy enemy(Coordinate(10, 10), EnemyType::AGGRESSIVE_MONSTER);
        CHECK(enemy.getHealth() == 2);
        CHECK(enemy.isActive() == true);
        
        enemy.takeDamage(1);
        CHECK(enemy.getHealth() == 1);
        CHECK(enemy.isActive() == true);
        
        enemy.takeDamage(1);
        CHECK(enemy.getHealth() == 0);
        CHECK(enemy.getIsDestroyed() == true);
    }
}

TEST_CASE("Rock Physics and Crushing") {
    SUBCASE("Rock initialization and basic physics") {
        Rock rock(Coordinate(8, 5));
        CHECK(rock.isActive() == true);
        CHECK(rock.getIsFalling() == false);
        CHECK(rock.getHasLanded() == false);
    }
    
    SUBCASE("Rock collision bounds") {
        Rock rock(Coordinate(10, 10));
        Coordinate bounds = rock.getCollisionBounds();
        CHECK(bounds.row == 1);
        CHECK(bounds.col == 1);
    }
}

TEST_CASE("Harpoon Movement and Behavior") {
    Player player(Coordinate(5, 5));
    
    SUBCASE("Harpoon creation and initial state") {
        Harpoon harpoon(Coordinate(5, 5), Direction::RIGHT, &player);
        CHECK(harpoon.isActive() == true);
        CHECK(harpoon.getDirection() == Direction::RIGHT);
        CHECK(harpoon.getState() == HarpoonState::EXTENDING);
    }
}

TEST_CASE("Boundary Restrictions") {
    SUBCASE("Top boundary") {
        Player player(Coordinate(0, 0));
        BlockGrid terrain;
        
        bool movedUp = player.moveInDirection(Direction::UP, terrain);
        CHECK(movedUp == false);
        CHECK(player.getPosition().row == 0);
    }
    
    SUBCASE("Corner boundaries") {
        Player player(Coordinate(0, 0));
        BlockGrid terrain;
        
        player.reset(Coordinate(0, 0));
        CHECK(player.moveInDirection(Direction::UP, terrain) == false);
        CHECK(player.moveInDirection(Direction::LEFT, terrain) == false);
        
        player.reset(Coordinate(19, 29));
        CHECK(player.moveInDirection(Direction::DOWN, terrain) == false);
        CHECK(player.moveInDirection(Direction::RIGHT, terrain) == false);
    }
}

TEST_CASE("Screen Environment") {
    SUBCASE("Screen dimensions") {
        const int CELL_SIZE = 40;
        const int SCREEN_WIDTH = 1200;
        const int SCREEN_HEIGHT = 800;
        
        CHECK(SCREEN_WIDTH / CELL_SIZE == Coordinate::WORLD_COLS);
        CHECK(SCREEN_HEIGHT / CELL_SIZE == Coordinate::WORLD_ROWS);
    }
}
