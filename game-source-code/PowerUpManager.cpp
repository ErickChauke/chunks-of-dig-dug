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
            std::cout << "Power-up expired: ";
            handlePowerUpExpiration(it->type);
            it = activePowerUps.erase(it);
        } else {
            ++it;
        }
    }
    
    hasRapidFire = hasPowerUpEffect(PowerUpType::RAPID_FIRE);
    hasPowerShot = hasPowerUpEffect(PowerUpType::POWER_SHOT);
    harpoonCooldown = hasRapidFire ? 0.3f : 0.8f;
}

void PowerUpManager::collectPowerUp(const PowerUp& powerUp, Player& player, 
                                   int& playerLives, int& score) {
    lastPowerUpCollected = GetTime();
    
    switch (powerUp.getType()) {
        case PowerUpType::EXTRA_LIFE:
            playerLives++;
            powerUpMessage = "+1 LIFE!";
            std::cout << "Extra Life! Lives: " << playerLives << std::endl;
            break;
            
        case PowerUpType::SCORE_MULTIPLIER:
            score += 500;
            powerUpMessage = "+500 BONUS!";
            std::cout << "Score Bonus! +500 points" << std::endl;
            break;
            
        case PowerUpType::RAPID_FIRE:
            activePowerUps.emplace_back(PowerUpType::RAPID_FIRE, 15.0f);
            powerUpMessage = "RAPID FIRE!";
            std::cout << "Rapid Fire activated for 15 seconds!" << std::endl;
            break;
            
        case PowerUpType::POWER_SHOT:
            activePowerUps.emplace_back(PowerUpType::POWER_SHOT, 20.0f);
            powerUpMessage = "POWER SHOT!";
            std::cout << "Power Shot activated for 20 seconds!" << std::endl;
            break;
            
        case PowerUpType::SPEED_BOOST:
            removeExistingPowerUp(PowerUpType::SPEED_BOOST);
            activePowerUps.emplace_back(PowerUpType::SPEED_BOOST, 12.0f);
            player.setSpeedMultiplier(0.5f);
            powerUpMessage = "SPEED BOOST!";
            std::cout << "Speed Boost activated for 12 seconds!" << std::endl;
            break;
            
        case PowerUpType::INVINCIBILITY:
            activePowerUps.emplace_back(PowerUpType::INVINCIBILITY, 10.0f);
            powerUpMessage = "INVINCIBLE!";
            std::cout << "Invincibility activated for 10 seconds!" << std::endl;
            break;
            
        default:
            score += 100;
            powerUpMessage = "+100 POINTS!";
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
            std::cout << "Rapid Fire expired" << std::endl;
            powerUpMessage = "Rapid Fire ended";
            lastPowerUpCollected = GetTime();
            break;
        case PowerUpType::POWER_SHOT:
            std::cout << "Power Shot expired" << std::endl;
            powerUpMessage = "Power Shot ended";
            lastPowerUpCollected = GetTime();
            break;
        case PowerUpType::SPEED_BOOST:
            std::cout << "Speed Boost expired - resetting to normal" << std::endl;
            powerUpMessage = "Speed normal";
            lastPowerUpCollected = GetTime();
            break;
        case PowerUpType::INVINCIBILITY:
            std::cout << "Invincibility expired" << std::endl;
            powerUpMessage = "Invincibility ended";
            lastPowerUpCollected = GetTime();
            break;
        default:
            break;
    }
}

void PowerUpManager::removeExistingPowerUp(PowerUpType type) {
    activePowerUps.erase(
        std::remove_if(activePowerUps.begin(), activePowerUps.end(),
            [type](const PowerUpEffect& effect) { return effect.type == type; }),
        activePowerUps.end()
    );
}

void PowerUpManager::reset() {
    activePowerUps.clear();
    lastPowerUpCollected = 0.0f;
    powerUpMessage = "";
    harpoonCooldown = 0.8f;
    hasRapidFire = false;
    hasPowerShot = false;
}

Player* PowerUpManager::getPlayerReference() {
    return playerRef;
}

void PowerUpManager::setPlayerReference(Player* player) {
    playerRef = player;
}

void PowerUpManager::applySpeedReset(Player& player) {
    if (!hasPowerUpEffect(PowerUpType::SPEED_BOOST)) {
        player.setSpeedMultiplier(1.0f);
        std::cout << "Speed reset to normal (1.0x)" << std::endl;
    }
}
