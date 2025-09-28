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
    
    // Add individual randomization for each enemy
    lastDecisionTime = GetTime() + (std::rand() % 50) * 0.01f;
}

Direction EnemyLogic::selectNextAction(Coordinate currentPos, Coordinate playerPos, 
                                      const BlockGrid& environment) {
    float currentTime = GetTime();
    
    // Update stuck counter if not moving effectively
    if (currentTime - lastDecisionTime > 1.2f) {
        if (previousMove != Direction::NONE) {
            stuckCounter++;
        }
        lastDecisionTime = currentTime;
    }
    
    // Add randomness to decision making to prevent synchronization
    int randomBehavior = std::rand() % 100;
    
    // If stuck for too long, try random movement
    if (stuckCounter > 2 || randomBehavior < 15) { // 15% chance of random behavior
        stuckCounter = 0;
        Direction randomDir = getRandomDirection();
        previousMove = randomDir;
        return randomDir;
    }
    
    // Calculate distance to player
    float distanceToPlayer = currentPos.calculateDistance(playerPos);
    
    // Add some randomness to movement decisions
    if (randomBehavior < 25) { // 25% chance of slightly random movement
        Direction towardPlayer = findDirectionToward(currentPos, playerPos);
        
        // Sometimes pick a perpendicular direction for more interesting movement
        if (std::rand() % 3 == 0) {
            if (towardPlayer == Direction::UP || towardPlayer == Direction::DOWN) {
                towardPlayer = (std::rand() % 2 == 0) ? Direction::LEFT : Direction::RIGHT;
            } else if (towardPlayer == Direction::LEFT || towardPlayer == Direction::RIGHT) {
                towardPlayer = (std::rand() % 2 == 0) ? Direction::UP : Direction::DOWN;
            }
        }
        
        previousMove = towardPlayer;
        return towardPlayer;
    }
    
    // Use different strategies based on distance and aggression
    if (distanceToPlayer < 6.0f || isAggressive) {
        // Close range: direct pursuit
        Direction towardPlayer = findDirectionToward(currentPos, playerPos);
        
        if (towardPlayer != Direction::NONE) {
            previousMove = towardPlayer;
            stuckCounter = 0;
            return towardPlayer;
        }
    } else {
        // Long range: use pathfinding with some randomness
        std::vector<Coordinate> path = findPathToPlayer(currentPos, playerPos, environment);
        
        if (path.size() > 1) {
            Coordinate nextPos = path[1]; // First step in path
            Direction moveDir = findDirectionToward(currentPos, nextPos);
            
            if (moveDir != Direction::NONE) {
                previousMove = moveDir;
                stuckCounter = 0;
                return moveDir;
            }
        }
    }
    
    // Fallback: move toward player directly
    Direction fallbackDir = findDirectionToward(currentPos, playerPos);
    previousMove = fallbackDir;
    return fallbackDir;
}

bool EnemyLogic::shouldPhaseThrough(Coordinate currentPos, Coordinate playerPos, 
                                   const BlockGrid& environment) {
    // Enemies can phase through earth blocks in Dig Dug style
    float distanceToPlayer = currentPos.calculateDistance(playerPos);
    
    // More likely to phase when close to player
    if (distanceToPlayer < 4.0f) {
        return true;
    }
    
    // Phase through if stuck
    if (stuckCounter > 1) {
        return true;
    }
    
    // Random chance to phase for unpredictability
    return (std::rand() % 100) < 30;
}

std::vector<Coordinate> EnemyLogic::findPathToPlayer(Coordinate start, Coordinate target, 
                                                   const BlockGrid& environment) {
    std::vector<Coordinate> path;
    
    // Simple pathfinding - can be enhanced with A*
    Coordinate current = start;
    int maxSteps = 8; // Prevent infinite loops
    
    while (current != target && maxSteps > 0) {
        Direction bestDir = findDirectionToward(current, target);
        
        if (bestDir == Direction::NONE) {
            break;
        }
        
        Coordinate nextPos = current + getDirectionOffset(bestDir);
        
        if (!nextPos.isWithinBounds()) {
            break;
        }
        
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
    
    // Add some randomness to prevent all enemies moving identically
    if (std::abs(deltaRow) == std::abs(deltaCol) && std::abs(deltaRow) > 0) {
        // When distances are equal, randomly choose direction
        return (std::rand() % 2 == 0) ? 
               ((deltaRow > 0) ? Direction::DOWN : Direction::UP) :
               ((deltaCol > 0) ? Direction::RIGHT : Direction::LEFT);
    }
    
    // Prioritize the larger difference for more direct movement
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
