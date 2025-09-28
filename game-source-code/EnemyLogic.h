#ifndef ENEMYLOGIC_H
#define ENEMYLOGIC_H

#include "Coordinate.h"
#include "BlockGrid.h"
#include <vector>

/**
 * @brief Direction enumeration for movement
 */
enum class Direction {
    UP,
    DOWN,
    LEFT,
    RIGHT,
    NONE
};

/**
 * @brief Makes movement decisions for enemies
 * 
 * The EnemyLogic class handles AI behavior for monsters,
 * including pathfinding, phase-through mechanics, and
 * decision making based on player position.
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
     * @brief Construct a new EnemyLogic object
     */
    EnemyLogic();
    
    /**
     * @brief Select the next movement direction for an enemy
     * @param currentPos Current enemy position
     * @param playerPos Current player position
     * @param environment Game environment grid
     * @return Direction Next movement direction
     */
    Direction selectNextAction(Coordinate currentPos, Coordinate playerPos, 
                              const BlockGrid& environment);
    
    /**
     * @brief Determine if enemy should phase through walls
     * @param currentPos Current enemy position
     * @param playerPos Current player position
     * @param environment Game environment grid
     * @return true if should phase through, false otherwise
     */
    bool shouldPhaseThrough(Coordinate currentPos, Coordinate playerPos, 
                           const BlockGrid& environment);
    
    /**
     * @brief Find optimal path to player using A* pathfinding
     * @param start Starting position
     * @param target Target position
     * @param environment Game environment
     * @return std::vector<Coordinate> Path to target
     */
    std::vector<Coordinate> findPathToPlayer(Coordinate start, Coordinate target, 
                                           const BlockGrid& environment);
    
    /**
     * @brief Set aggressive behavior mode
     * @param aggressive Whether enemy should be more aggressive
     */
    void setAggressive(bool aggressive);

private:
    /**
     * @brief Get coordinate offset for a direction
     * @param dir Direction to get offset for
     * @return Coordinate Offset for the direction
     */
    Coordinate getDirectionOffset(Direction dir) const;
    
    /**
     * @brief Find best direction toward target
     * @param from Starting position
     * @param to Target position
     * @return Direction Best direction to move
     */
    Direction findDirectionToward(Coordinate from, Coordinate to) const;
    
    /**
     * @brief Check if position is safe for enemy movement
     * @param pos Position to check
     * @param environment Game environment
     * @return true if safe, false otherwise
     */
    bool isSafePosition(Coordinate pos, const BlockGrid& environment) const;
    
    /**
     * @brief Calculate heuristic for A* pathfinding
     * @param from Starting position
     * @param to Target position
     * @return int Heuristic value
     */
    int calculateHeuristic(Coordinate from, Coordinate to) const;
    
    /**
     * @brief Get random direction when stuck
     * @return Direction Random valid direction
     */
    Direction getRandomDirection() const;
};

#endif // ENEMYLOGIC_H
