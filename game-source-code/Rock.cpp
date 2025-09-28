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
    std::cout << "Rock created at (" << pos.row << "," << pos.col << ")" << std::endl;
}

void Rock::update() {
    // Only update crush timer for actively falling rocks
    if (isFalling) {
        crushTimer += GetFrameTime();
    } else {
        // Reset crush timer for stationary rocks
        crushTimer = 0.0f;
    }
}

void Rock::render() {
    // Rendering handled by main game loop
}

bool Rock::hasSupport(const BlockGrid& terrain) const {
    Coordinate below = position + Coordinate(1, 0);
    
    // Bottom of world is always solid
    if (below.row >= Coordinate::WORLD_ROWS) {
        return true;
    }
    
    return terrain.isLocationBlocked(below);
}

void Rock::checkStability(const BlockGrid& terrain) {
    stabilityCheckTimer += GetFrameTime();
    
    // Check stability every 0.1 seconds
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
    
    // Only fall if no support and actively falling
    if (!hasSupport(terrain) && isFalling) {
        fallTimer += GetFrameTime();
        
        if (fallTimer >= FALL_SPEED) {
            Coordinate oldPos = position;
            Coordinate newPos = position + Coordinate(1, 0);
            
            // Check if new position is valid
            if (newPos.row >= Coordinate::WORLD_ROWS || terrain.isLocationBlocked(newPos)) {
                std::cout << "Rock landed at (" << newPos.row << "," << newPos.col << ")" << std::endl;
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
    // IMPORTANT: Only falling rocks can crush! Stationary rocks are safe.
    if (!isFalling) {
        return; // Stationary rocks cannot crush anything
    }
    
    // Update player movement tracking only for falling rocks
    updatePlayerMovementTracking(player);
    
    // Check if rock crushes enemies - apply to all enemies in path
    for (auto& enemy : enemies) {
        if (enemy.isActive() && !enemy.getIsDestroyed() && checkEnemyCrush(enemy)) {
            std::cout << "Falling rock crushes enemy at (" << enemy.getPosition().row 
                      << "," << enemy.getPosition().col << ")" << std::endl;
            enemy.destroy();
        }
    }
}

bool Rock::checkPlayerCrush(const Player& player) {
    // CRITICAL: Only falling rocks can crush players
    if (!isFalling) {
        return false;
    }
    
    Coordinate playerPos = player.getPosition();
    Coordinate rockPos = this->position;
    
    // FIXED: Direct collision - rock must be at EXACT same position as player
    if (playerPos.row == rockPos.row && playerPos.col == rockPos.col) {
        std::cout << "DIRECT HIT: Falling rock landed exactly on player position!" << std::endl;
        return true;
    }
    
    // FIXED: Only check crush if rock is directly above AND in same column
    if (rockPos.col == playerPos.col && rockPos.row < playerPos.row) {
        // Check if player is actively moving away from danger
        bool playerMoving = isPlayerMovingAwayFromRock(player);
        float activeCrushDelay = getActiveCrushDelay();
        
        if (playerMoving) {
            crushTimer = 0.0f; // Reset timer if player is actively escaping
            return false;
        }
        
        // Apply crush delay based on player movement for falling rocks only
        if (crushTimer >= activeCrushDelay) {
            std::cout << "CRUSH TIMEOUT: Player stayed under falling rock too long!" << std::endl;
            return true;
        }
    } else {
        // Player not under falling rock - reset crush timer
        crushTimer = 0.0f;
    }
    
    return false;
}

bool Rock::checkEnemyCrush(const Enemy& enemy) const {
    // Only falling rocks can crush enemies
    if (!isFalling) {
        return false;
    }
    
    Coordinate enemyPos = enemy.getPosition();
    Coordinate rockPos = this->position;
    
    // FIXED: Exact position match required for crush
    if (enemyPos.row == rockPos.row && enemyPos.col == rockPos.col) {
        return true;
    }
    
    // FIXED: Also crush if falling rock is directly above enemy (same column)
    if (rockPos.col == enemyPos.col && rockPos.row < enemyPos.row) {
        // Enemies get crushed if rock is anywhere above them in same column
        return true;
    }
    
    return false;
}

bool Rock::isDirectlyAbove(Coordinate target) const {
    // Check if rock is in same column and above the target
    return (position.col == target.col && position.row < target.row);
}

bool Rock::isPlayerMovingAwayFromRock(const Player& player) {
    // Check if player is actively digging/moving
    if (player.getIsDigging() || player.getIsMoving()) {
        Coordinate currentPos = player.getPosition();
        
        // If player moved to a different column, they're escaping horizontally
        if (currentPos.col != this->position.col) {
            return true;
        }
        
        // If player is digging down rapidly (moving down the column)
        if (currentPos.row > lastPlayerPosition.row && player.getIsDigging()) {
            return true;
        }
        
        // Any active movement counts as escape attempt from falling rock
        if (player.getIsMoving()) {
            return true;
        }
    }
    
    return false;
}

void Rock::updatePlayerMovementTracking(const Player& player) {
    // Only track movement for falling rocks
    if (!isFalling) {
        return;
    }
    
    float currentTime = GetTime();
    
    if (currentTime - lastPlayerCheckTime >= PLAYER_MOVE_CHECK_INTERVAL) {
        Coordinate currentPos = player.getPosition();
        
        // Update movement tracking
        if (lastPlayerPosition.row != -1) { // Not first check
            playerIsMovingAway = isPlayerMovingAwayFromRock(player);
        }
        
        lastPlayerPosition = currentPos;
        lastPlayerCheckTime = currentTime;
    }
}

float Rock::getActiveCrushDelay() const {
    // Use shorter delay for stationary players, longer for moving players
    return playerIsMovingAway ? CRUSH_DELAY : STATIONARY_CRUSH_DELAY;
}

float Rock::getCrushTimeRemaining() const {
    // Only falling rocks have crush timers
    if (!isFalling) {
        return 0.0f;
    }
    
    float activeCrushDelay = getActiveCrushDelay();
    return crushTimer < activeCrushDelay ? activeCrushDelay - crushTimer : 0.0f;
}

Coordinate Rock::getCollisionBounds() const {
    return Coordinate(1, 1);
}

void Rock::onCollision(GameObject* other) {
    // Minimal collision logging to reduce spam
    if (other && isFalling) {
        // Only log important collisions
    }
}

void Rock::startFalling() {
    if (!isFalling) {
        isFalling = true;
        crushTimer = 0.0f;
        hasLanded = false;
        playerIsMovingAway = false;
        std::cout << "Rock starts falling at (" << position.row << "," << position.col << ")" << std::endl;
    }
}

void Rock::stopFalling() {
    if (isFalling) {
        isFalling = false;
        crushTimer = 0.0f;
        hasLanded = true;
        playerIsMovingAway = false;
        std::cout << "Rock stops falling at (" << position.row << "," << position.col << ")" << std::endl;
    }
}
