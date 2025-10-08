#ifndef POWERUPMANAGER_H
#define POWERUPMANAGER_H

#include "PowerUp.h"
#include "Player.h"
#include "UIManager.h"
#include <vector>
#include <string>

/**
 * @file PowerUpManager.h
 * @brief Manages active power-up effects and player stat modifications
 */

/**
 * @class PowerUpManager
 * @brief Tracks and applies timed power-up effects
 * 
 * PowerUpManager coordinates power-up collection and effect lifecycle:
 * - Applies immediate effects (lives, score)
 * - Tracks active timed effects (speed, rapid fire, power shot)
 * - Automatically expires effects after duration
 * - Modifies player stats during active effects
 * - Prevents effect stacking (resets timers on recollection)
 * 
 * Effect types managed:
 * - EXTRA_LIFE: Immediate +1 life
 * - SCORE_MULTIPLIER: Immediate +500 points
 * - RAPID_FIRE: Harpoon cooldown 0.3s (15s duration)
 * - POWER_SHOT: Enhanced harpoon damage (20s duration)
 * - SPEED_BOOST: Movement cooldown 0.5x (12s duration)
 * - INVINCIBILITY: Collision immunity (10s duration)
 * 
 * Lifecycle:
 * 1. Collection: PowerUp triggers collectPowerUp()
 * 2. Application: Stats modified, effect added to vector
 * 3. Update: Each frame checks for expirations
 * 4. Expiration: Stats reset, effect removed
 * 
 * @note Manager prevents duplicate effects by removing old instances
 */
class PowerUpManager {
private:
    std::vector<PowerUpEffect> activePowerUps;
    float lastPowerUpCollected;
    std::string powerUpMessage;
    float harpoonCooldown;
    bool hasRapidFire;
    bool hasPowerShot;
    Player* playerRef;

public:
    PowerUpManager();
    
    /**
     * @brief Update all active effects and check expirations
     */
    void update();
    
    /**
     * @brief Process power-up collection
     * @param powerUp Collected power-up
     * @param player Player to modify
     * @param playerLives Lives counter (modified for EXTRA_LIFE)
     * @param score Score counter (modified for bonuses)
     */
    void collectPowerUp(const PowerUp& powerUp, Player& player, 
                       int& playerLives, int& score);
    
    /**
     * @brief Check if specific effect is active
     * @param type Power-up type to check
     * @return true if effect currently active
     */
    bool hasPowerUpEffect(PowerUpType type) const;
    
    float getHarpoonCooldown() const { return harpoonCooldown; }
    bool getHasRapidFire() const { return hasRapidFire; }
    bool getHasPowerShot() const { return hasPowerShot; }
    
    const std::vector<PowerUpEffect>& getActivePowerUps() const { 
        return activePowerUps; 
    }
    
    std::string getPowerUpMessage() const { return powerUpMessage; }
    float getTimeSinceLastCollection() const;
    
    /**
     * @brief Reset all effects and clear active list
     */
    void reset();
    
    /**
     * @brief Reset player speed if no active speed boost
     * @param player Player to reset
     */
    void applySpeedReset(Player& player);
    
    void setPlayerReference(Player* player);
    Player* getPlayerReference();

private:
    void handlePowerUpExpiration(PowerUpType type);
    void updatePowerUpEffects();
    void removeExistingPowerUp(PowerUpType type);
};

#endif // POWERUPMANAGER_H
