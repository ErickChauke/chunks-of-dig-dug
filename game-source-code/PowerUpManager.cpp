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

void PowerUpManager::collectPowerUp(const PowerUp& powerUp, Player& player, 
                                   int& playerLives, int& score) {
    lastPowerUpCollected = GetTime();
    
    switch (powerUp.getType()) {
        case PowerUpType::EXTRA_LIFE:
            playerLives++;
            powerUpMessage = "Extra Life Gained!";
            std::cout << "Extra Life! Lives: " << playerLives << std::endl;
            break;
            
        case PowerUpType::SCORE_MULTIPLIER:
            score += 500;
            powerUpMessage = "+500 Score Bonus!";
            std::cout << "Score Bonus! +500 points" << std::endl;
            break;
            
        case PowerUpType::RAPID_FIRE:
            activePowerUps.emplace_back(PowerUpType::RAPID_FIRE, 15.0f);
            powerUpMessage = "Rapid Fire Activated!";
            std::cout << "Rapid Fire activated for 15 seconds!" << std::endl;
            break;
            
        case PowerUpType::POWER_SHOT:
            activePowerUps.emplace_back(PowerUpType::POWER_SHOT, 20.0f);
            powerUpMessage = "Power Shot Activated!";
            std::cout << "Power Shot activated for 20 seconds!" << std::endl;
            break;
            
        case PowerUpType::SPEED_BOOST:
            activePowerUps.emplace_back(PowerUpType::SPEED_BOOST, 12.0f);
            player.setSpeedMultiplier(0.6f);
            powerUpMessage = "Speed Boost Activated!";
            std::cout << "Speed Boost activated for 12 seconds!" << std::endl;
            break;
            
        case PowerUpType::INVINCIBILITY:
            activePowerUps.emplace_back(PowerUpType::INVINCIBILITY, 10.0f);
            powerUpMessage = "Invincibility Activated!";
            std::cout << "Invincibility activated for 10 seconds!" << std::endl;
            break;
            
        default:
            score += 100;
            powerUpMessage = "+100 Points!";
            break;
    }
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
