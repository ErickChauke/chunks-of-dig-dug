#include "Harpoon.h"
#include "Player.h"
#include <raylib-cpp.hpp>

const float HARPOON_SPEED = 4.0f;
const float HARPOON_MAX_RANGE = 3.0f; // 3 blocks range

Harpoon::Harpoon(Coordinate startPos, Direction dir, Player* player) 
    : GameObject(startPos), direction(dir), speed(HARPOON_SPEED), 
      maxRange(HARPOON_MAX_RANGE), currentLength(0.0f), 
      state(HarpoonState::EXTENDING), startPosition(startPos),
      playerRef(player) {
    segments.push_back(startPos);
}

void Harpoon::update() {
    if (!active) return;
    
    // Always update connection to player first
    updatePlayerConnection();
    
    switch (state) {
        case HarpoonState::EXTENDING:
            extend();
            break;
        case HarpoonState::RETRACTING:
            retract();
            break;
        case HarpoonState::IDLE:
            break;
    }
}

void Harpoon::render() {
    // Rendering handled by main game loop
}

bool Harpoon::checkEnemyHit(Coordinate enemyPos) {
    for (const auto& segment : segments) {
        if (segment == enemyPos) {
            state = HarpoonState::RETRACTING;
            return true;
        }
    }
    return false;
}

Direction Harpoon::getDirection() const {
    return direction;
}

HarpoonState Harpoon::getState() const {
    return state;
}

const std::vector<Coordinate>& Harpoon::getSegments() const {
    return segments;
}

void Harpoon::updatePlayerConnection() {
    if (playerRef && !segments.empty()) {
        // Update the start position to current player position
        startPosition = playerRef->getPosition();
        segments[0] = startPosition;
        
        // Recalculate all segments to maintain connection
        recalculateSegments();
    }
}

void Harpoon::extend() {
    float deltaTime = GetFrameTime();
    currentLength += speed * deltaTime;
    
    if (currentLength >= 1.0f) {
        Coordinate offset;
        switch (direction) {
            case Direction::UP:    offset = Coordinate(-1, 0); break;
            case Direction::DOWN:  offset = Coordinate(1, 0); break;
            case Direction::LEFT:  offset = Coordinate(0, -1); break;
            case Direction::RIGHT: offset = Coordinate(0, 1); break;
            case Direction::NONE:  return;
        }
        
        Coordinate nextPos = segments.back() + offset;
        
        if (!nextPos.isWithinBounds() || segments.size() >= maxRange) {
            state = HarpoonState::RETRACTING;
            return;
        }
        
        segments.push_back(nextPos);
        position = nextPos;
        currentLength = 0.0f;
    }
}

void Harpoon::retract() {
    float deltaTime = GetFrameTime();
    currentLength += speed * deltaTime;
    
    if (currentLength >= 1.0f && segments.size() > 1) {
        segments.pop_back();
        if (!segments.empty()) {
            position = segments.back();
        }
        currentLength = 0.0f;
        
        if (segments.size() <= 1) {
            setActive(false);
        }
    }
}

void Harpoon::recalculateSegments() {
    if (segments.size() <= 1) return;
    
    // Keep the direction and length, but recalculate from new player position
    Coordinate offset;
    switch (direction) {
        case Direction::UP:    offset = Coordinate(-1, 0); break;
        case Direction::DOWN:  offset = Coordinate(1, 0); break;
        case Direction::LEFT:  offset = Coordinate(0, -1); break;
        case Direction::RIGHT: offset = Coordinate(0, 1); break;
        case Direction::NONE:  return;
    }
    
    // Rebuild segments from player position
    int segmentCount = segments.size();
    segments.clear();
    segments.push_back(startPosition);
    
    for (int i = 1; i < segmentCount; ++i) {
        Coordinate nextPos = segments.back() + offset;
        if (nextPos.isWithinBounds()) {
            segments.push_back(nextPos);
        } else {
            // Hit boundary, start retracting
            state = HarpoonState::RETRACTING;
            break;
        }
    }
    
    if (!segments.empty()) {
        position = segments.back();
    }
}
