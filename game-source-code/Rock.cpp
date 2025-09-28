#include "Rock.h"
#include "Player.h"
#include "Enemy.h"
#include <raylib-cpp.hpp>
#include <iostream>

Rock::Rock(Coordinate pos) : GameObject(pos), fallTimer(0.0f), stabilityCheckTimer(0.0f),
                            crushTimer(0.0f), lastPlayerCheckTime(0.0f), 
                            lastPlayerPosition(-1, -1), isFalling(false), hasLanded(false),
                            playerIsMovingAway(false) {
    setActive(true);
}

void Rock::update() {
    if (isFalling) {
        crushTimer += GetFrameTime();
    } else {
        crushTimer = 0.0f;
    }
}

void Rock::render() {
    // Rendering handled by main game loop
}

bool Rock::hasSupport(const BlockGrid& terrain) const {
    Coordinate below = position + Coordinate(1, 0);
    if (below.row >= Coordinate::WORLD_ROWS) {
        return true;
    }
    return terrain.isLocationBlocked(below);
}

void Rock::checkStability(const BlockGrid& terrain) {
    stabilityCheckTimer += GetFrameTime();
    
    if (stabilityCheckTimer >= 0.1f) {
        bool hasSupp = hasSupport(terrain);
        
        if (!hasSupp && !isFalling) {
            startFalling();
        } else if (hasSupp && isFalling) {
            stopFalling();
        }
        
        stabilityCheckTimer = 0.0f;
    }
}

void Rock::applyGravity(const BlockGrid& terrain) {
    checkStability(terrain);
    
    if (!hasSupport(terrain) && isFalling) {
        fallTimer += GetFrameTime();
        
        if (fallTimer >= FALL_SPEED) {
            Coordinate newPos = position + Coordinate(1, 0);
            
            if (newPos.row >= Coordinate::WORLD_ROWS || terrain.isLocationBlocked(newPos)) {
                stopFalling();
                hasLanded = true;
                return;
            }
            
            position = newPos;
            fallTimer = 0.0f;
        }
    }
}

void Rock::handleCrushingLogic(const Player& player, std::vector<Enemy>& enemies) {
    if (!isFalling) {
        return;
    }
    
    updatePlayerMovementTracking(player);
    
    for (auto& enemy : enemies) {
        if (enemy.isActive() && !enemy.getIsDestroyed() && checkEnemyCrush(enemy)) {
            enemy.destroy();
        }
    }
}

bool Rock::checkPlayerCrush(const Player& player) {
    if (!isFalling) {
        return false;
    }
    
    Coordinate playerPos = player.getPosition();
    if (playerPos.row == position.row && playerPos.col == position.col) {
        return true;
    }
    
    return false;
}

bool Rock::checkEnemyCrush(const Enemy& enemy) const {
    if (!isFalling) return false;
    
    Coordinate enemyPos = enemy.getPosition();
    if (enemyPos.row == position.row && enemyPos.col == position.col) {
        return true;
    }
    
    return false;
}

bool Rock::isDirectlyAbove(Coordinate target) const {
    return (position.col == target.col && position.row < target.row);
}

bool Rock::isPlayerMovingAwayFromRock(const Player& player) {
    return player.getIsDigging() || player.getIsMoving();
}

void Rock::updatePlayerMovementTracking(const Player& player) {
    if (!isFalling) return;
    
    float currentTime = GetTime();
    
    if (currentTime - lastPlayerCheckTime >= PLAYER_MOVE_CHECK_INTERVAL) {
        Coordinate currentPos = player.getPosition();
        
        if (lastPlayerPosition.row != -1) {
            playerIsMovingAway = isPlayerMovingAwayFromRock(player);
        }
        
        lastPlayerPosition = currentPos;
        lastPlayerCheckTime = currentTime;
    }
}

float Rock::getActiveCrushDelay() const {
    return playerIsMovingAway ? CRUSH_DELAY : STATIONARY_CRUSH_DELAY;
}

float Rock::getCrushTimeRemaining() const {
    if (!isFalling) return 0.0f;
    float activeCrushDelay = getActiveCrushDelay();
    return crushTimer < activeCrushDelay ? activeCrushDelay - crushTimer : 0.0f;
}

Coordinate Rock::getCollisionBounds() const {
    return Coordinate(1, 1);
}

void Rock::onCollision(GameObject* other) {
    // Collision handling
}

void Rock::startFalling() {
    if (!isFalling) {
        isFalling = true;
        crushTimer = 0.0f;
        hasLanded = false;
        playerIsMovingAway = false;
    }
}

void Rock::stopFalling() {
    if (isFalling) {
        isFalling = false;
        crushTimer = 0.0f;
        hasLanded = true;
        playerIsMovingAway = false;
    }
}
