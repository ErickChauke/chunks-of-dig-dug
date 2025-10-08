#ifndef POWERUP_H
#define POWERUP_H

#include "GameObject.h"
#include "Coordinate.h"

/**
 * @file PowerUp.h
 * @brief Collectible power-ups with timed effects
 */

/**
 * @enum PowerUpType
 * @brief Different power-up variants
 */
enum class PowerUpType {
    EXTRA_LIFE,       ///< +1 life (permanent)
    SCORE_MULTIPLIER, ///< Bonus points (permanent)
    SPEED_BOOST,      ///< 2x movement speed (12s)
    INVINCIBILITY,    ///< Immune to damage (8s)
    RAPID_FIRE,       ///< Faster harpoon cooldown (15s)
    POWER_SHOT        ///< Enhanced harpoon damage (15s)
};

/**
 * @class PowerUp
 * @brief Temporary or permanent player enhancement
 * 
 * PowerUps spawn periodically and grant various effects:
 * 
 * Permanent effects:
 * - EXTRA_LIFE: +1 life
 * - SCORE_MULTIPLIER: +500 points
 * 
 * Timed effects:
 * - SPEED_BOOST: Player moves twice as fast (12s)
 * - RAPID_FIRE: Harpoon cooldown 0.3s instead of 0.8s (15s)
 * - POWER_SHOT: Enhanced damage (15s)
 * - INVINCIBILITY: No collision damage (8s)
 * 
 * Lifecycle:
 * - Spawns at random valid position
 * - Lifetime: 30 seconds before despawning
 * - Collection: Triggers effect and sets collected flag
 * 
 * @note PowerUpManager tracks active timed effects
 */
class PowerUp : public GameObject, public Collidable {
private:
    PowerUpType type;
    int value;
    float duration;
    float spawnTime;
    float lifetime;
    bool collected;

public:
    /**
     * @brief Construct power-up at position
     * @param pos Spawn coordinate
     * @param powerType Power-up variant
     */
    PowerUp(Coordinate pos, PowerUpType powerType);
    
    void update() override;
    void render() override;
    
    Coordinate getCollisionBounds() const override;
    void onCollision(GameObject* other) override;
    
    PowerUpType getType() const { return type; }
    int getValue() const { return value; }
    float getDuration() const { return duration; }
    bool isCollected() const { return collected; }
    
    /**
     * @brief Mark power-up as collected
     */
    void collect();
    
    /**
     * @brief Check if should despawn
     * @return true if lifetime exceeded
     */
    bool shouldDespawn() const;

private:
    void initializePowerUp();
};

#endif // POWERUP_H
