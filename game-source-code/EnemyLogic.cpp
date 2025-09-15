#include "EnemyLogic.h"
#include <cstdlib>
#include <ctime>

EnemyLogic::EnemyLogic() : previousMove(Direction::NONE), blockedCount(0) {
    static bool seeded = false;
    if (!seeded) {
        std::srand(static_cast<unsigned>(std::time(nullptr)));
        seeded = true;
    }
}

Direction EnemyLogic::selectNextAction(Coordinate currentPos, Coordinate playerPos, 
                                      const BlockGrid& environment) {
    Direction towardPlayer = findDirectionToward(currentPos, playerPos);
    Coordinate nextPos = currentPos + getDirectionOffset(towardPlayer);
    
    if (!environment.isLocationBlocked(nextPos)) {
        blockedCount = 0;
        previousMove = towardPlayer;
        return towardPlayer;
    }
    
    blockedCount++;
    Direction directions[] = {Direction::UP, Direction::DOWN, Direction::LEFT, Direction::RIGHT};
    
    for (int i = 0; i < 4; ++i) {
        Direction dir = directions[i];
        Coordinate testPos = currentPos + getDirectionOffset(dir);
        if (!environment.isLocationBlocked(testPos)) {
            previousMove = dir;
            return dir;
        }
    }
    
    return Direction::NONE;
}

bool EnemyLogic::shouldPhaseThrough(Coordinate currentPos, Coordinate playerPos, 
                                   const BlockGrid& environment) {
    return blockedCount > 5;
}

Coordinate EnemyLogic::getDirectionOffset(Direction dir) const {
    switch (dir) {
        case Direction::UP:    return Coordinate(-1, 0);
        case Direction::DOWN:  return Coordinate(1, 0);
        case Direction::LEFT:  return Coordinate(0, -1);
        case Direction::RIGHT: return Coordinate(0, 1);
        case Direction::NONE:  return Coordinate(0, 0);
    }
    return Coordinate(0, 0);
}

Direction EnemyLogic::findDirectionToward(Coordinate from, Coordinate to) const {
    int deltaRow = to.row - from.row;
    int deltaCol = to.col - from.col;
    
    if (std::abs(deltaRow) > std::abs(deltaCol)) {
        return (deltaRow > 0) ? Direction::DOWN : Direction::UP;
    } else {
        return (deltaCol > 0) ? Direction::RIGHT : Direction::LEFT;
    }
}
