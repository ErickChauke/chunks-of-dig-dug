#include "PowerUp.h"
#include <raylib-cpp.hpp>

PowerUp::PowerUp(Coordinate pos, PowerUpType powerType) 
    : GameObject(pos), type(powerType), value(0), duration(0.0f),
      spawnTime(GetTime()), lifetime(30.0f), collected(false) {
    initializePowerUp();
}

void PowerUp::update() {
    if (!active || collected) return;
    
    // Check if power-up should despawn
    if (shouldDespawn()) {
        setActive(false);
    }
}

void PowerUp::render() {
    // Rendering handled by main game loop
}

Coordinate PowerUp::getCollisionBounds() const {
    return Coordinate(1, 1);
}

void PowerUp::onCollision(GameObject* other) {
    // Handle collision with player
    if (other && !collected) {
        collect();
    }
}

void PowerUp::collect() {
    if (!collected) {
        collected = true;
        setActive(false);
    }
}

bool PowerUp::shouldDespawn() const {
    float currentTime = GetTime();
    return (currentTime - spawnTime) >= lifetime;
}

void PowerUp::initializePowerUp() {
    switch (type) {
        case PowerUpType::EXTRA_LIFE:
            value = 1;
            break;
        case PowerUpType::SCORE_MULTIPLIER:
            value = 2;
            duration = 15.0f;
            break;
        case PowerUpType::SPEED_BOOST:
            value = 2;
            duration = 10.0f;
            break;
        case PowerUpType::INVINCIBILITY:
            value = 1;
            duration = 8.0f;
            break;
        case PowerUpType::RAPID_FIRE:
            value = 1;
            duration = 12.0f;
            break;
        case PowerUpType::POWER_SHOT:
            value = 1;
            duration = 15.0f;
            break;
    }
}
