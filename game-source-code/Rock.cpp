#include "Rock.h"
#include "Player.h"
#include "Enemy.h"
#include "GameConstants.h"
#include <raylib-cpp.hpp>
#include <iostream>

using namespace GameConstants;

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
    
    if (stabilityCheckTimer >= ROCK_STABILITY_CHECK_INTERVAL) {
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
        
        if (fallTimer >= ROCK_FALL_SPEED) {
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
    if (!isFalling) return;
    
    updatePlayerMovementTracking(player);
    
    for (auto& enemy : enemies) {
        if (enemy.isActive() && !enemy.getIsDestroyed() && checkEnemyCrush(enemy)) {
            enemy.destroy();
        }
    }
}

bool Rock::checkPlayerCrush(const Player& player) {
    if (!isFalling) return false;
    
    Coordinate playerPos = player.getPosition();
    
    if (isDirectHit(playerPos)) {
        return true;
    }
    
    if (isPlayerInDangerZone(playerPos)) {
        bool playerMoving = (player.getIsDigging() || player.getIsMoving());
        float activeCrushDelay = getActiveCrushDelay();
        
        if (playerMoving) {
            crushTimer = 0.0f;
            return false;
        }
        
        if (crushTimer >= activeCrushDelay) {
            return true;
        }
    } else if (shouldResetCrushTimer(playerPos)) {
        crushTimer = 0.0f;
    }
    
    return false;
}

bool Rock::checkEnemyCrush(const Enemy& enemy) const {
    if (!isFalling) return false;
    
    Coordinate enemyPos = enemy.getPosition();
    return (enemyPos == position) || 
           (position.col == enemyPos.col && position.row < enemyPos.row);
}

bool Rock::isDirectHit(const Coordinate& playerPos) const {
    return playerPos == position;
}

bool Rock::isPlayerInDangerZone(const Coordinate& playerPos) const {
    return position.col == playerPos.col && position.row < playerPos.row;
}

bool Rock::shouldResetCrushTimer(const Coordinate& playerPos) const {
    return position.col != playerPos.col || position.row >= playerPos.row;
}

void Rock::updatePlayerMovementTracking(const Player& player) {
    if (!isFalling) return;
    
    float currentTime = GetTime();
    
    if (currentTime - lastPlayerCheckTime >= ENEMY_MOVE_CHECK_INTERVAL) {
        if (lastPlayerPosition.row != -1) {
            playerIsMovingAway = (player.getIsDigging() || player.getIsMoving());
        }
        
        lastPlayerPosition = player.getPosition();
        lastPlayerCheckTime = currentTime;
    }
}

float Rock::getActiveCrushDelay() const {
    return playerIsMovingAway ? ROCK_CRUSH_DELAY : ROCK_STATIONARY_CRUSH_DELAY;
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
