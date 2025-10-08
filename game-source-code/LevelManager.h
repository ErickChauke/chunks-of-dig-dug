#ifndef LEVELMANAGER_H
#define LEVELMANAGER_H

#include "BlockGrid.h"
#include "Player.h"
#include "Enemy.h"
#include "PowerUp.h"
#include "Rock.h"
#include "Coordinate.h"
#include <vector>

/**
 * @file LevelManager.h
 * @brief Level progression and entity spawning system
 */

/**
 * @class LevelManager
 * @brief Manages level initialization, progression, and spawning
 * 
 * LevelManager coordinates level-based gameplay mechanics:
 * - Loading terrain layouts from map files
 * - Spawning enemies, rocks, and power-ups
 * - Tracking level completion conditions
 * - Calculating time-based bonuses
 * - Progressive difficulty scaling
 * 
 * Level progression system:
 * - Level 1-2: Use custom map files (level1.txt, level2.txt)
 * - Level 3+: Procedural default maps
 * - Enemy count: 3 + level (capped at 8)
 * - Rock count: 4 + level/2 (capped at 10)
 * - Target score: Increases by 1000 per level
 * 
 * Spawning strategy:
 * - Enemies: Randomized types with GREEN_DRAGON and RED_MONSTER guaranteed
 * - Rocks: Spread across 3 sections, avoid player start area
 * - PowerUps: Spawn every 20-30 seconds during play
 * 
 * Completion conditions:
 * - All enemies defeated OR target score reached
 * - Time bonus awarded based on completion speed
 * 
 * @note Manages all level-scoped entity lifecycles
 */
class LevelManager {
private:
    int currentLevel;
    int targetScore;
    float nextPowerUpTime;

public:
    LevelManager();
    
    /**
     * @brief Initialize level with terrain and entities
     * @param level Level number to initialize
     * @param terrain Terrain grid to populate
     * @param player Player to reset
     * @param enemies Enemy vector to populate
     * @param powerUps PowerUp vector to clear
     * @param rocks Rock vector to populate
     */
    void initializeLevel(int level, BlockGrid& terrain, Player& player, 
                        std::vector<Enemy>& enemies, std::vector<PowerUp>& powerUps,
                        std::vector<Rock>& rocks);
    
    /**
     * @brief Check if should spawn power-up
     * @param levelTimer Current level time elapsed
     * @return true if spawn time reached
     */
    bool shouldSpawnPowerUp(float levelTimer) const;
    
    /**
     * @brief Create random power-up at valid position
     * @return PowerUp Newly created power-up
     */
    PowerUp createRandomPowerUp();
    
    /**
     * @brief Update next power-up spawn time
     * @param levelTimer Current level time
     */
    void updatePowerUpSpawnTime(float levelTimer);
    
    /**
     * @brief Check if level completion conditions met
     * @param enemies Active enemies
     * @param score Current score
     * @return true if level complete
     */
    bool isLevelComplete(const std::vector<Enemy>& enemies, int score) const;
    
    /**
     * @brief Calculate time-based completion bonus
     * @param levelTimer Time taken to complete level
     * @param level Current level number
     * @return int Bonus score (0-1000+)
     */
    int calculateTimeBonus(float levelTimer, int level) const;
    
    /**
     * @brief Progress to next level
     */
    void nextLevel();
    
    /**
     * @brief Reset to level 1
     */
    void reset();
    
    int getCurrentLevel() const { return currentLevel; }
    int getTargetScore() const { return targetScore; }

private:
    void spawnEnemies(std::vector<Enemy>& enemies);
    void spawnRocks(std::vector<Rock>& rocks, const BlockGrid& terrain);
    Coordinate findValidSpawnPosition(const Player& player) const;
    Coordinate findValidRockPosition(const std::vector<Coordinate>& existingRocks) const;
    std::string getLevelMapFile(int level) const;
};

#endif // LEVELMANAGER_H
