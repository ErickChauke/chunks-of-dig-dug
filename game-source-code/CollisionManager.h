#ifndef COLLISIONMANAGER_H
#define COLLISIONMANAGER_H

#include "Player.h"
#include "Enemy.h"
#include "Harpoon.h"
#include "PowerUp.h"
#include "Rock.h"
#include <vector>

/**
 * @file CollisionManager.h
 * @brief Centralized collision detection and response
 */

/**
 * @class CollisionManager
 * @brief Coordinates collision detection across all entity types
 * 
 * CollisionManager implements the Manager pattern to centralize
 * cross-cutting collision concerns. Entities don't need to know
 * about each other's types - the manager handles all interactions.
 * 
 * Collision types handled:
 * - Player vs Enemy (game over)
 * - Harpoon vs Enemy (enemy destroyed, score awarded)
 * - Rock vs Player (game over if crushed)
 * - Rock vs Enemy (enemy destroyed)
 * - Player vs PowerUp (power-up collected)
 * 
 * Benefits of centralization:
 * - Single responsibility: Entities focus on behavior
 * - Global view: Can check all combinations efficiently
 * - Loose coupling: New entities easily added
 * - Testable: Collision logic isolated
 * 
 * Detection methods:
 * - AABB (Axis-Aligned Bounding Box) for general collisions
 * - Position matching for grid-based collisions
 * 
 * @note Manager has global view of all entities each frame
 */
class CollisionManager {
public:
    CollisionManager() = default;
    
    /**
     * @brief Check player-enemy collisions
     * @param player Player reference
     * @param enemies Enemy vector
     * @return true if collision detected
     */
    bool checkPlayerEnemyCollision(const Player& player, 
                                  const std::vector<Enemy>& enemies);
    
    /**
     * @brief Check harpoon-enemy collisions
     * @param harpoons Harpoon vector (const - read only)
     * @param enemies Enemy vector (modified if hit)
     * @param score Score counter (incremented on hits)
     * @param enemiesDefeated Total enemies defeated (incremented)
     * @param level Current level (for score multiplier)
     */
    void checkHarpoonEnemyCollisions(std::vector<Harpoon>& harpoons, 
                                   std::vector<Enemy>& enemies, int& score, 
                                   int& enemiesDefeated, int level);
    
    /**
     * @brief Check rock crushing collisions
     * @param rocks Rock vector
     * @param player Player reference
     * @param enemies Enemy vector (modified if crushed)
     * @param playerCrushed Output: true if player crushed
     */
    void checkRockCollisions(std::vector<Rock>& rocks, const Player& player, 
                           std::vector<Enemy>& enemies, bool& playerCrushed);
    
    /**
     * @brief Check player-powerup collisions
     * @param player Player reference
     * @param powerUps PowerUp vector
     * @return PowerUp* Pointer to collected power-up (nullptr if none)
     */
    PowerUp* checkPowerUpCollision(const Player& player, 
                                  std::vector<PowerUp>& powerUps);

private:
    int getScoreForEnemy(EnemyType type, int level);
    bool checkAABBCollision(Coordinate pos1, Coordinate bounds1,
                           Coordinate pos2, Coordinate bounds2);
    bool isPositionMatch(Coordinate pos1, Coordinate pos2);
};

#endif // COLLISIONMANAGER_H
