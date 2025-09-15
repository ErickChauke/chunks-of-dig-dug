#ifndef ENEMYLOGIC_H
#define ENEMYLOGIC_H

#include "Coordinate.h"
#include "BlockGrid.h"

enum class Direction {
    UP,
    DOWN,
    LEFT,
    RIGHT,
    NONE
};

class EnemyLogic {
private:
    Direction previousMove;
    int blockedCount;

public:
    EnemyLogic();
    Coordinate getDirectionOffset(Direction dir) const;
    Direction findDirectionToward(Coordinate from, Coordinate to) const;
};

#endif
