#include "Player.h"
#include <raylib-cpp.hpp>
#include <iostream>

const float MOVE_COOLDOWN = 0.15f;

Player::Player(Coordinate startPos) 
    : GameObject(startPos), moveTimer(0.0f), isDigging(false), 
      lastMoveDirection(Direction::NONE), tunnelsCreated(0), 
      isMoving(false), moveCooldown(MOVE_COOLDOWN) {
}

void Player::update() {
}

void Player::render() {
}

bool Player::handleMovement(BlockGrid& terrain) {
    Direction inputDir = Direction::NONE;
    
    if (IsKeyDown(KEY_UP))    inputDir = Direction::UP;
    if (IsKeyDown(KEY_DOWN))  inputDir = Direction::DOWN;
    if (IsKeyDown(KEY_LEFT))  inputDir = Direction::LEFT;
    if (IsKeyDown(KEY_RIGHT)) inputDir = Direction::RIGHT;
    
    if (inputDir != Direction::NONE && canMove()) {
        bool moved = moveInDirection(inputDir, terrain);
        if (moved) {
            moveTimer = GetTime();
            isMoving = true;
        }
        return moved;
    }
    
    isMoving = false;
    return false;
}

bool Player::canMove() const {
    return (GetTime() - moveTimer) >= moveCooldown;
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
    if (terrain.isLocationBlocked(pos)) {
        terrain.clearPassageAt(pos);
        tunnelsCreated++;
        isDigging = true;
        return true;
    }
    return false;
}

Coordinate Player::getCollisionBounds() const { return Coordinate(1, 1); }
void Player::onCollision(GameObject* other) {}
bool Player::getIsDigging() const { return isDigging; }
int Player::getTunnelsCreated() const { return tunnelsCreated; }
Direction Player::getLastMoveDirection() const { return lastMoveDirection; }
bool Player::getIsMoving() const { return isMoving; }

void Player::reset(Coordinate newPos) {
    position = newPos;
    moveTimer = 0.0f;
    tunnelsCreated = 0;
    isDigging = false;
    lastMoveDirection = Direction::NONE;
    isMoving = false;
    setActive(true);
}
