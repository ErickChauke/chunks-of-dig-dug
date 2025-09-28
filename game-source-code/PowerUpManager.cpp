#include "PowerUpManager.h"
#include <raylib-cpp.hpp>
#include <iostream>
#include <algorithm>

PowerUpManager::PowerUpManager() : lastPowerUpCollected(0.0f), powerUpMessage(""),
                                  harpoonCooldown(1.0f), hasRapidFire(false), 
                                  hasPowerShot(false) {
}

void PowerUpManager::update() {
    updatePowerUpEffects();
}

void PowerUpManager::updatePowerUpEffects() {
    for (auto it = activePowerUps.begin(); it != activePowerUps.end();) {
        if (it->isExpired()) {
            handlePowerUpExpiration(it->type);
            it = activePowerUps.erase(it);
        } else {
            ++it;
        }
    }
    
    hasRapidFire = hasPowerUpEffect(PowerUpType::RAPID_FIRE);
    hasPowerShot = hasPowerUpEffect(PowerUpType::POWER_SHOT);
    harpoonCooldown = hasRapidFire ? 0.3f : 1.0f;
}

bool PowerUpManager::hasPowerUpEffect(PowerUpType type) const {
    for (const auto& effect : activePowerUps) {
        if (effect.type == type && effect.active) {
            return true;
        }
    }
    return false;
}

float PowerUpManager::getTimeSinceLastCollection() const {
    return GetTime() - lastPowerUpCollected;
}

void PowerUpManager::handlePowerUpExpiration(PowerUpType type) {
    switch (type) {
        case PowerUpType::RAPID_FIRE:
            powerUpMessage = "Rapid Fire Expired";
            lastPowerUpCollected = GetTime();
            break;
        case PowerUpType::POWER_SHOT:
            powerUpMessage = "Power Shot Expired";
            lastPowerUpCollected = GetTime();
            break;
        case PowerUpType::SPEED_BOOST:
            powerUpMessage = "Speed Boost Expired";
            lastPowerUpCollected = GetTime();
            break;
        case PowerUpType::INVINCIBILITY:
            powerUpMessage = "Invincibility Expired";
            lastPowerUpCollected = GetTime();
            break;
        default:
            break;
    }
}

void PowerUpManager::reset() {
    activePowerUps.clear();
    lastPowerUpCollected = 0.0f;
    powerUpMessage = "";
    harpoonCooldown = 1.0f;
    hasRapidFire = false;
    hasPowerShot = false;
}
