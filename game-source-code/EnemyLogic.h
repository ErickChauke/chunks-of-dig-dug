#ifndef ENEMYLOGIC_H
#define ENEMYLOGIC_H

#include "Coordinate.h"
#include "BlockGrid.h"
#include <vector>

enum class Direction {
    UP,
    DOWN,
    LEFT,
    RIGHT,
    NONE
};

/**
 * @brief Makes movement decisions for enemies
 */
class EnemyLogic {
private:
    Direction previousMove;
    int blockedCount;
    int stuckCounter;
    float lastDecisionTime;
    bool isAggressive;

public:
    EnemyLogic();
    Direction selectNextAction(Coordinate currentPos, Coordinate playerPos, 
                              const BlockGrid& environment);
    bool shouldPhaseThrough(Coordinate currentPos, Coordinate playerPos, 
                           const BlockGrid& environment);
    std::vector<Coordinate> findPathToPlayer(Coordinate start, Coordinate target, 
                                           const BlockGrid& environment);
    void setAggressive(bool aggressive);

private:
    Coordinate getDirectionOffset(Direction dir) const;
    Direction findDirectionToward(Coordinate from, Coordinate to) const;
    bool isSafePosition(Coordinate pos, const BlockGrid& environment) const;
    int calculateHeuristic(Coordinate from, Coordinate to) const;
    Direction getRandomDirection() const;
};

#endif // ENEMYLOGIC_H
