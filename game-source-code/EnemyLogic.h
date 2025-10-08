#ifndef ENEMYLOGIC_H
#define ENEMYLOGIC_H

#include "Coordinate.h"
#include "BlockGrid.h"
#include <vector>

/**
 * @file EnemyLogic.h
 * @brief AI decision-making system for enemy movement
 */

/**
 * @enum Direction
 * @brief Movement directions for grid-based navigation
 */
enum class Direction {
    UP,    ///< Move one cell up (row - 1)
    DOWN,  ///< Move one cell down (row + 1)
    LEFT,  ///< Move one cell left (col - 1)
    RIGHT, ///< Move one cell right (col + 1)
    NONE   ///< No movement
};

/**
 * @class EnemyLogic
 * @brief AI controller for enemy pathfinding
 * 
 * Implements hybrid intelligent/random AI behavior:
 * - 15-25% random movement (prevents predictability)
 * - Stuck detection and recovery
 * - Distance-based strategy (close=direct, far=pathfinding)
 * - Randomized timing per enemy (prevents synchronization)
 * 
 * @note Each Enemy has its own EnemyLogic instance
 */
class EnemyLogic {
private:
    Direction previousMove;
    int blockedCount;
    int stuckCounter;
    float lastDecisionTime;
    bool isAggressive;

public:
    /**
     * @brief Construct AI controller with randomized timing
     */
    EnemyLogic();
    
    /**
     * @brief Determine next movement direction
     * @param currentPos Enemy's current position
     * @param playerPos Player's current position
     * @param environment Game terrain for pathfinding
     * @return Direction Best direction to move
     */
    Direction selectNextAction(Coordinate currentPos, Coordinate playerPos, 
                              const BlockGrid& environment);
    
    /**
     * @brief Check if enemy should phase through walls
     * @param currentPos Enemy position
     * @param playerPos Player position
     * @param environment Game terrain
     * @return true if should phase through
     */
    bool shouldPhaseThrough(Coordinate currentPos, Coordinate playerPos, 
                           const BlockGrid& environment);
    
    /**
     * @brief Find path to player using pathfinding
     * @param start Starting position
     * @param target Target position
     * @param environment Game environment
     * @return std::vector<Coordinate> Path to target
     */
    std::vector<Coordinate> findPathToPlayer(Coordinate start, Coordinate target, 
                                           const BlockGrid& environment);
    
    /**
     * @brief Enable aggressive pursuit behavior
     * @param aggressive true for aggressive mode
     */
    void setAggressive(bool aggressive);

private:
    Coordinate getDirectionOffset(Direction dir) const;
    Direction findDirectionToward(Coordinate from, Coordinate to) const;
    bool isSafePosition(Coordinate pos, const BlockGrid& environment) const;
    int calculateHeuristic(Coordinate from, Coordinate to) const;
    Direction getRandomDirection() const;
};

#endif // ENEMYLOGIC_H
