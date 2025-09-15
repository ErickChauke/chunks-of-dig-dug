#include "Player.h"
#include <raylib-cpp.hpp>
#include <iostream>
#include <algorithm>

const float BASE_MOVE_COOLDOWN = 0.15f;
const float FAST_MOVE_COOLDOWN = 0.10f;
const float DIG_EFFECT_DURATION = 0.3f;
const int CONSECUTIVE_MOVES_FOR_SPEEDUP = 5;

Player::Player(Coordinate startPos) 
    : GameObject(startPos), moveTimer(0.0f), digEffectTimer(0.0f), 
      isDigging(false), lastMoveDirection(Direction::NONE), 
      currentInputDirection(Direction::NONE), tunnelsCreated(0), 
      moveCooldown(BASE_MOVE_COOLDOWN), isMoving(false), speedMultiplier(1.0f),
      consecutiveMoves(0), canDigDiagonally(false) {
}

void Player::update() {
    updateMovementTimer();
    updateDiggingEffects();
    updateMovementStats();
}

void Player::render() {
}

bool Player::handleMovement(BlockGrid& terrain) {
    Direction inputDirection = processInputBuffer();
    
    if (inputDirection == Direction::NONE || !canMove()) {
        isMoving = false;
        return false;
    }
    
    bool moved = moveInDirection(inputDirection, terrain);
    
    if (moved) {
        moveTimer = GetTime();
        isMoving = true;
        consecutiveMoves++;
        moveCooldown = getDynamicMoveCooldown();
    } else {
        isMoving = false;
        consecutiveMoves = 0;
    }
    
    return moved;
}

bool Player::moveInDirection(Direction direction, BlockGrid& terrain) {
    Coordinate offset;
    
    switch (direction) {
        case Direction::UP:    offset = Coordinate(-1, 0); break;
        case Direction::DOWN:  offset = Coordinate(1, 0); break;
        case Direction::LEFT:  offset = Coordinate(0, -1); break;
        case Direction::RIGHT: offset = Coordinate(0, 1); break;
        case Direction::NONE:  return false;
    }
    
    Coordinate newPos = position + offset;
    
    if (!newPos.isWithinBounds()) {
        return false;
    }
    
    lastMoveDirection = direction;
    currentInputDirection = direction;
    
    if (terrain.isLocationBlocked(newPos)) {
        if (digTunnel(newPos, terrain)) {
            position = newPos;
            return true;
        }
        return false;
    }
    
    position = newPos;
    return true;
}

bool Player::digTunnel(Coordinate pos, BlockGrid& terrain) {
    if (!pos.isWithinBounds()) {
        return false;
    }
    
    if (terrain.isLocationBlocked(pos)) {
        terrain.clearPassageAt(pos);
        tunnelsCreated++;
        isDigging = true;
        digEffectTimer = GetTime();
        return true;
    }
    
    return false;
}

void Player::updateMovementTimer() {}

void Player::updateDiggingEffects() {
    if (isDigging) {
        float currentTime = GetTime();
        if (currentTime - digEffectTimer >= DIG_EFFECT_DURATION) {
            isDigging = false;
        }
    }
}

void Player::updateMovementStats() {
    static float lastMoveTime = 0.0f;
    float currentTime = GetTime();
    
    if (!isMoving && (currentTime - lastMoveTime) > 1.0f) {
        if (consecutiveMoves > 0) {
            consecutiveMoves = std::max(0, consecutiveMoves - 1);
        }
    }
    
    if (isMoving) {
        lastMoveTime = currentTime;
    }
}

bool Player::canMove() const {
    float currentTime = GetTime();
    float effectiveCooldown = moveCooldown * speedMultiplier;
    return (currentTime - moveTimer) >= effectiveCooldown;
}

Direction Player::processInputBuffer() {
    if (IsKeyDown(KEY_UP)) return Direction::UP;
    if (IsKeyDown(KEY_DOWN)) return Direction::DOWN;
    if (IsKeyDown(KEY_LEFT)) return Direction::LEFT;
    if (IsKeyDown(KEY_RIGHT)) return Direction::RIGHT;
    return Direction::NONE;
}

float Player::getDynamicMoveCooldown() const {
    if (consecutiveMoves >= CONSECUTIVE_MOVES_FOR_SPEEDUP) {
        return FAST_MOVE_COOLDOWN;
    }
    
    float speedupFactor = 1.0f - (consecutiveMoves * 0.02f);
    return BASE_MOVE_COOLDOWN * std::max(0.7f, speedupFactor);
}

Coordinate Player::getCollisionBounds() const { return Coordinate(1, 1); }
void Player::onCollision(GameObject* other) { if (other && other->isActive()) consecutiveMoves = 0; }
bool Player::getIsDigging() const { return isDigging; }
int Player::getTunnelsCreated() const { return tunnelsCreated; }
Direction Player::getLastMoveDirection() const { return lastMoveDirection; }
Direction Player::getCurrentInputDirection() const { return currentInputDirection; }
bool Player::getIsMoving() const { return isMoving; }
float Player::getSpeedMultiplier() const { return speedMultiplier; }

void Player::setSpeedMultiplier(float multiplier) {
    speedMultiplier = std::max(0.1f, std::min(5.0f, multiplier));
}

void Player::setDiagonalDigging(bool enabled) {
    canDigDiagonally = enabled;
}

void Player::reset(Coordinate newPos) {
    position = newPos;
    moveTimer = 0.0f;
    digEffectTimer = 0.0f;
    isDigging = false;
    lastMoveDirection = Direction::NONE;
    currentInputDirection = Direction::NONE;
    tunnelsCreated = 0;
    isMoving = false;
    consecutiveMoves = 0;
    moveCooldown = BASE_MOVE_COOLDOWN;
    setActive(true);
}
