#include "EnemyLogic.h"
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <queue>
#include <map>
#include <raylib-cpp.hpp>

EnemyLogic::EnemyLogic() : previousMove(Direction::NONE), blockedCount(0), 
                          stuckCounter(0), lastDecisionTime(0.0f), isAggressive(false) {
    static bool seeded = false;
    if (!seeded) {
        std::srand(static_cast<unsigned>(std::time(nullptr)));
        seeded = true;
    }
    lastDecisionTime = GetTime() + (std::rand() % 50) * 0.01f;
}

Direction EnemyLogic::selectNextAction(Coordinate currentPos, Coordinate playerPos, 
                                      const BlockGrid& environment) {
    float currentTime = GetTime();
    
    if (currentTime - lastDecisionTime > 1.2f) {
        if (previousMove != Direction::NONE) {
            stuckCounter++;
        }
        lastDecisionTime = currentTime;
    }
    
    int randomBehavior = std::rand() % 100;
    
    if (stuckCounter > 2 || randomBehavior < 15) {
        stuckCounter = 0;
        Direction randomDir = getRandomDirection();
        previousMove = randomDir;
        return randomDir;
    }
    
    Direction towardPlayer = findDirectionToward(currentPos, playerPos);
    previousMove = towardPlayer;
    return towardPlayer;
}

bool EnemyLogic::shouldPhaseThrough(Coordinate currentPos, Coordinate playerPos, 
                                   const BlockGrid& environment) {
    float distanceToPlayer = currentPos.calculateDistance(playerPos);
    return distanceToPlayer < 4.0f || stuckCounter > 1 || (std::rand() % 100) < 30;
}

std::vector<Coordinate> EnemyLogic::findPathToPlayer(Coordinate start, Coordinate target, 
                                                   const BlockGrid& environment) {
    std::vector<Coordinate> path;
    Coordinate current = start;
    int maxSteps = 8;
    
    while (current != target && maxSteps > 0) {
        Direction bestDir = findDirectionToward(current, target);
        if (bestDir == Direction::NONE) break;
        
        Coordinate nextPos = current + getDirectionOffset(bestDir);
        if (!nextPos.isWithinBounds()) break;
        
        path.push_back(nextPos);
        current = nextPos;
        maxSteps--;
    }
    return path;
}

void EnemyLogic::setAggressive(bool aggressive) {
    isAggressive = aggressive;
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
    
    if (std::abs(deltaRow) == std::abs(deltaCol) && std::abs(deltaRow) > 0) {
        return (std::rand() % 2 == 0) ? 
               ((deltaRow > 0) ? Direction::DOWN : Direction::UP) :
               ((deltaCol > 0) ? Direction::RIGHT : Direction::LEFT);
    }
    
    if (std::abs(deltaRow) > std::abs(deltaCol)) {
        return (deltaRow > 0) ? Direction::DOWN : Direction::UP;
    } else if (std::abs(deltaCol) > 0) {
        return (deltaCol > 0) ? Direction::RIGHT : Direction::LEFT;
    }
    return Direction::NONE;
}

bool EnemyLogic::isSafePosition(Coordinate pos, const BlockGrid& environment) const {
    return pos.isWithinBounds();
}

int EnemyLogic::calculateHeuristic(Coordinate from, Coordinate to) const {
    return from.manhattanDistance(to);
}

Direction EnemyLogic::getRandomDirection() const {
    Direction directions[] = {Direction::UP, Direction::DOWN, Direction::LEFT, Direction::RIGHT};
    return directions[std::rand() % 4];
}
