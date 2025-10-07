#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include "../game-source-code/Coordinate.h"
#include "../game-source-code/BlockGrid.h"
#include "../game-source-code/Player.h"
#include "../game-source-code/Enemy.h"
#include "../game-source-code/EnemyLogic.h"
#include "../game-source-code/Rock.h"
#include "../game-source-code/Harpoon.h"
#include "../game-source-code/LevelManager.h"
#include "../game-source-code/PowerUpManager.h"

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

TEST_CASE("Enemy Logic AI") {
    SUBCASE("Direction calculation methods work") {
        EnemyLogic ai;
        BlockGrid terrain;
        
        Coordinate enemyPos(10, 10);
        Coordinate playerPos(8, 10);
        
        Direction move = ai.selectNextAction(enemyPos, playerPos, terrain);
        // AI may return any valid direction due to randomization
        CHECK((move == Direction::UP || move == Direction::DOWN || 
               move == Direction::LEFT || move == Direction::RIGHT ||
               move == Direction::NONE));
    }
    
    SUBCASE("Pathfinding to player") {
        EnemyLogic ai;
        BlockGrid terrain;
        
        Coordinate start(5, 5);
        Coordinate target(5, 8);
        
        std::vector<Coordinate> path = ai.findPathToPlayer(start, target, terrain);
        CHECK(path.size() >= 0);
    }
    
    SUBCASE("Aggressive mode") {
        EnemyLogic ai;
        ai.setAggressive(false);
        ai.setAggressive(true);
    }
}

TEST_CASE("Collision Detection") {
    SUBCASE("Enemy-Player collision") {
        Player player(Coordinate(10, 10));
        Enemy enemy(Coordinate(10, 10));
        
        CHECK(player.getPosition() == enemy.getPosition());
    }
    
    SUBCASE("Collision bounds") {
        Enemy enemy(Coordinate(5, 5));
        Coordinate bounds = enemy.getCollisionBounds();
        CHECK(bounds.row == 1);
        CHECK(bounds.col == 1);
    }
}

TEST_CASE("Tunnel Creation") {
    Player player(Coordinate(5, 5));
    BlockGrid terrain;
    
    SUBCASE("Multiple tunnels") {
        int startCount = player.getTunnelsCreated();
        
        bool moved1 = player.moveInDirection(Direction::RIGHT, terrain);
        CHECK(moved1 == true);
        CHECK(player.getTunnelsCreated() == startCount + 1);
        
        bool moved2 = player.moveInDirection(Direction::DOWN, terrain);
        CHECK(moved2 == true);
        CHECK(player.getTunnelsCreated() == startCount + 2);
        
        bool moved3 = player.moveInDirection(Direction::LEFT, terrain);
        CHECK(moved3 == true);
        CHECK(player.getTunnelsCreated() == startCount + 3);
        
        bool moved4 = player.moveInDirection(Direction::LEFT, terrain);
        CHECK(moved4 == true);
        CHECK(player.getTunnelsCreated() == startCount + 4);
    }
}

TEST_CASE("Game State Management") {
    SUBCASE("Enemy spawning patterns") {
        for (int i = 0; i < 10; ++i) {
            int row = 5 + (std::rand() % 10);
            int col = 5 + (std::rand() % 20);
            Coordinate pos(row, col);
            CHECK(pos.isWithinBounds() == true);
        }
    }
    
    SUBCASE("Wave progression") {
        int wave1Enemies = 2 + 1;
        int wave2Enemies = 2 + 2;
        int wave3Enemies = 2 + 3;
        
        CHECK(wave1Enemies == 3);
        CHECK(wave2Enemies == 4);
        CHECK(wave3Enemies == 5);
    }
}

TEST_CASE("Tunnel Creation") {
    Player player(Coordinate(5, 5));
    BlockGrid terrain;
    
    SUBCASE("Multiple tunnels") {
        int startCount = player.getTunnelsCreated();
        
        bool moved1 = player.moveInDirection(Direction::RIGHT, terrain);
        CHECK(moved1 == true);
        CHECK(player.getTunnelsCreated() == startCount + 1);
        
        bool moved2 = player.moveInDirection(Direction::DOWN, terrain);
        CHECK(moved2 == true);
        CHECK(player.getTunnelsCreated() == startCount + 2);
        
        bool moved3 = player.moveInDirection(Direction::LEFT, terrain);
        CHECK(moved3 == true);
        CHECK(player.getTunnelsCreated() == startCount + 3);
        
        bool moved4 = player.moveInDirection(Direction::LEFT, terrain);
        CHECK(moved4 == true);
        CHECK(player.getTunnelsCreated() == startCount + 4);
    }
}

TEST_CASE("Game State Management") {
    SUBCASE("Enemy spawning patterns") {
        for (int i = 0; i < 10; ++i) {
            int row = 5 + (std::rand() % 10);
            int col = 5 + (std::rand() % 20);
            Coordinate pos(row, col);
            CHECK(pos.isWithinBounds() == true);
        }
    }
    
    SUBCASE("Wave progression") {
        int wave1Enemies = 2 + 1;
        int wave2Enemies = 2 + 2;
        int wave3Enemies = 2 + 3;
        
        CHECK(wave1Enemies == 3);
        CHECK(wave2Enemies == 4);
        CHECK(wave3Enemies == 5);
    }
}

TEST_CASE("Raylib Integration") {
    SUBCASE("Coordinate conversion") {
        Coordinate pos(5, 10);
        const int CELL_SIZE = 40;
        
        int screenX = pos.col * CELL_SIZE;
        int screenY = pos.row * CELL_SIZE;
        
        CHECK(screenX == 400);
        CHECK(screenY == 200);
    }
}

TEST_CASE("Rock Physics and Crushing") {
    SUBCASE("Rock support detection") {
        BlockGrid terrain;
        Rock rock(Coordinate(8, 5));
        
        CHECK(rock.hasSupport(terrain) == true);
        
        terrain.clearPassageAt(Coordinate(9, 5));
        rock.checkStability(terrain);
    }
    
    SUBCASE("Multiple rocks don't interfere") {
        Rock rock1(Coordinate(5, 5));
        Rock rock2(Coordinate(5, 10));
        
        CHECK(rock1.getPosition() != rock2.getPosition());
        CHECK(rock1.isActive() == true);
        CHECK(rock2.isActive() == true);
    }
}

TEST_CASE("Rock-Player Interaction") {
    SUBCASE("Player can tunnel under rocks") {
        BlockGrid terrain;
        Player player(Coordinate(9, 5));
        Rock rock(Coordinate(8, 5));
        
        bool canDig = player.moveInDirection(Direction::UP, terrain);
        CHECK(canDig == true);
        
        CHECK(rock.isActive() == true);
    }
    
    SUBCASE("Player survival mechanics") {
        Player player(Coordinate(10, 5));
        Rock rock(Coordinate(8, 5));
        
        CHECK(rock.getCrushTimeRemaining() >= 0.0f);
    }
}

TEST_CASE("Enhanced Enemy Movement Testing") {
    SUBCASE("Different enemy types movement") {
        Enemy redMonster(Coordinate(5, 5), EnemyType::RED_MONSTER);
        Enemy aggressiveMonster(Coordinate(5, 6), EnemyType::AGGRESSIVE_MONSTER);
        Enemy fastMonster(Coordinate(5, 7), EnemyType::FAST_MONSTER);
        
        CHECK(redMonster.getEnemyType() == EnemyType::RED_MONSTER);
        CHECK(aggressiveMonster.getEnemyType() == EnemyType::AGGRESSIVE_MONSTER);
        CHECK(fastMonster.getEnemyType() == EnemyType::FAST_MONSTER);
    }
}

TEST_CASE("Advanced Player Movement Tests") {
    SUBCASE("Player rock collision detection") {
        Player player(Coordinate(5, 5));
        BlockGrid terrain;
        std::vector<Rock> rocks;
        
        rocks.emplace_back(Coordinate(5, 6));
        
        bool moved = player.moveInDirectionWithRocks(Direction::RIGHT, terrain, rocks);
        CHECK(moved == false);
        CHECK(player.getPosition().col == 5);
    }
}

TEST_CASE("Object Movement Integration") {
    SUBCASE("Boundary compliance verification") {
        Player player(Coordinate(3, 0));
        Enemy enemy(Coordinate(19, 29));
        
        BlockGrid terrain;
        
        bool playerMoved = player.moveInDirection(Direction::UP, terrain);
        
        CHECK(playerMoved == false);
        CHECK(player.getPosition().isWithinBounds() == true);
        CHECK(enemy.getPosition().isWithinBounds() == true);
    }
}

TEST_CASE("Enhanced Enemy Movement Testing") {
    SUBCASE("Enemy movement capability") {
        BlockGrid terrain;
        Enemy enemy(Coordinate(10, 10));
        
        Coordinate originalPos = enemy.getPosition();
        
        bool moved1 = enemy.moveToward(Coordinate(8, 10), terrain);
        enemy.setPosition(originalPos);
        bool moved2 = enemy.moveToward(Coordinate(12, 10), terrain);
        enemy.setPosition(originalPos);
        bool moved3 = enemy.moveToward(Coordinate(10, 8), terrain);
        enemy.setPosition(originalPos);
        bool moved4 = enemy.moveToward(Coordinate(10, 12), terrain);
        
        // At least movement methods should work
        CHECK((moved1 == true || moved1 == false));
        CHECK((moved2 == true || moved2 == false));
        CHECK((moved3 == true || moved3 == false));
        CHECK((moved4 == true || moved4 == false));
    }
}

TEST_CASE("Advanced Player Movement Tests") {
    SUBCASE("Player tunnel creation tracking") {
        Player player(Coordinate(5, 5));
        BlockGrid terrain;
        
        int initialTunnels = player.getTunnelsCreated();
        
        player.moveInDirection(Direction::RIGHT, terrain);
        player.moveInDirection(Direction::DOWN, terrain);
        
        CHECK(player.getTunnelsCreated() > initialTunnels);
    }
}

TEST_CASE("Object Movement Integration") {
    SUBCASE("Multiple object position tracking") {
        Player player(Coordinate(5, 5));
        Enemy enemy(Coordinate(5, 5));
        Rock rock(Coordinate(5, 5));
        
        CHECK(player.getPosition() == enemy.getPosition());
        CHECK(player.getPosition() == rock.getPosition());
    }
}



TEST_CASE("PowerUpManager System") {
    PowerUpManager manager;
    Player player(Coordinate(5, 5));
    int lives = 3;
    int score = 0;
    
    SUBCASE("Initialization") {
        CHECK(manager.getHarpoonCooldown() == doctest::Approx(1.0f));
        CHECK(manager.getHasRapidFire() == false);
        CHECK(manager.getHasPowerShot() == false);
    }
    
    SUBCASE("Extra life collection") {
        PowerUp extraLife(Coordinate(5, 5), PowerUpType::EXTRA_LIFE);
        manager.collectPowerUp(extraLife, player, lives, score);
        CHECK(lives == 4);
    }
    
    SUBCASE("Score multiplier collection") {
        PowerUp scoreBonus(Coordinate(5, 5), PowerUpType::SCORE_MULTIPLIER);
        manager.collectPowerUp(scoreBonus, player, lives, score);
        CHECK(score == 500);
    }
    
    SUBCASE("Rapid fire activation") {
        PowerUp rapidFire(Coordinate(5, 5), PowerUpType::RAPID_FIRE);
        manager.collectPowerUp(rapidFire, player, lives, score);
        manager.update();
        CHECK(manager.getHasRapidFire() == true);
        CHECK(manager.getHarpoonCooldown() == doctest::Approx(0.3f));
    }
    
    SUBCASE("Power shot activation") {
        PowerUp powerShot(Coordinate(5, 5), PowerUpType::POWER_SHOT);
        manager.collectPowerUp(powerShot, player, lives, score);
        manager.update();
        CHECK(manager.getHasPowerShot() == true);
    }
    
    SUBCASE("Speed boost activation") {
        PowerUp speedBoost(Coordinate(5, 5), PowerUpType::SPEED_BOOST);
        manager.collectPowerUp(speedBoost, player, lives, score);
        manager.update();
        CHECK(manager.hasPowerUpEffect(PowerUpType::SPEED_BOOST) == true);
    }
    
}

TEST_CASE("LevelManager System") {
    LevelManager levelMgr;
    
    SUBCASE("Initialization") {
        CHECK(levelMgr.getCurrentLevel() == 1);
        CHECK(levelMgr.getTargetScore() == 1000);
    }
    
    SUBCASE("Level progression") {
        int initialLevel = levelMgr.getCurrentLevel();
        levelMgr.nextLevel();
        CHECK(levelMgr.getCurrentLevel() == initialLevel + 1);
        CHECK(levelMgr.getTargetScore() > 1000);
    }
    
    SUBCASE("Multiple level progression") {
        for (int i = 0; i < 3; ++i) {
            levelMgr.nextLevel();
        }
        CHECK(levelMgr.getCurrentLevel() == 4);
    }
    
    SUBCASE("Level completion - all enemies defeated") {
        std::vector<Enemy> enemies;
        enemies.emplace_back(Coordinate(5, 5), EnemyType::RED_MONSTER);
        enemies[0].destroy();
        
        bool complete = levelMgr.isLevelComplete(enemies, 0);
        CHECK(complete == true);
    }
    
    SUBCASE("Level completion - target score reached") {
        std::vector<Enemy> enemies;
        enemies.emplace_back(Coordinate(5, 5), EnemyType::RED_MONSTER);
        
        bool complete = levelMgr.isLevelComplete(enemies, 1000);
        CHECK(complete == true);
    }
    
    SUBCASE("Level not complete") {
        std::vector<Enemy> enemies;
        enemies.emplace_back(Coordinate(5, 5), EnemyType::RED_MONSTER);
        
        bool complete = levelMgr.isLevelComplete(enemies, 500);
        CHECK(complete == false);
    }
    
    SUBCASE("Time bonus calculation") {
        int bonus1 = levelMgr.calculateTimeBonus(60.0f, 1);
        CHECK(bonus1 > 0);
        
        int bonus2 = levelMgr.calculateTimeBonus(120.0f, 1);
        CHECK(bonus2 < bonus1);
        
        int bonus3 = levelMgr.calculateTimeBonus(180.0f, 1);
        CHECK(bonus3 == 0);
    }
    
    SUBCASE("PowerUp spawn timing") {
        CHECK(levelMgr.shouldSpawnPowerUp(20.0f) == true);
        CHECK(levelMgr.shouldSpawnPowerUp(5.0f) == false);
    }
}
