#ifndef POWERUPMANAGER_H
#define POWERUPMANAGER_H

#include "PowerUp.h"
#include "Player.h"
#include "UIManager.h"
#include <vector>
#include <string>

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
    
    void update();
    void collectPowerUp(const PowerUp& powerUp, Player& player, 
                       int& playerLives, int& score);
    
    bool hasPowerUpEffect(PowerUpType type) const;
    float getHarpoonCooldown() const { return harpoonCooldown; }
    bool getHasRapidFire() const { return hasRapidFire; }
    bool getHasPowerShot() const { return hasPowerShot; }
    
    const std::vector<PowerUpEffect>& getActivePowerUps() const { 
        return activePowerUps; 
    }
    
    std::string getPowerUpMessage() const { return powerUpMessage; }
    float getTimeSinceLastCollection() const;
    
    void reset();
    void applySpeedReset(Player& player);
    void setPlayerReference(Player* player);
    Player* getPlayerReference();

private:
    void handlePowerUpExpiration(PowerUpType type);
    void updatePowerUpEffects();
    void removeExistingPowerUp(PowerUpType type);
};

#endif // POWERUPMANAGER_H
