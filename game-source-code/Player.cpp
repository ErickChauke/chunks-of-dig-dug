#include "Player.h"
#include <iostream>

Player::Player(Coordinate startPos) 
    : GameObject(startPos), moveTimer(0.0f), isDigging(false), 
      lastMoveDirection(Direction::NONE), tunnelsCreated(0), isMoving(false) {
}

void Player::update() {
    // Basic update
}

void Player::render() {
    // Rendering handled by main
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
    tunnelsCreated = 0;
    isDigging = false;
    lastMoveDirection = Direction::NONE;
    setActive(true);
}
