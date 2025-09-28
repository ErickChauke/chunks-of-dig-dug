#ifndef POWERUP_H
#define POWERUP_H

#include "GameObject.h"
#include "Coordinate.h"

enum class PowerUpType {
    EXTRA_LIFE,
    SCORE_MULTIPLIER,
    SPEED_BOOST,
    INVINCIBILITY,
    RAPID_FIRE,
    POWER_SHOT
};

class PowerUp : public GameObject, public Collidable {
private:
    PowerUpType type;
    int value;
    float duration;
    float spawnTime;
    float lifetime;
    bool collected;

public:
    PowerUp(Coordinate pos, PowerUpType powerType);
    
    void update() override;
    void render() override;
    
    Coordinate getCollisionBounds() const override;
    void onCollision(GameObject* other) override;
    
    PowerUpType getType() const { return type; }
    int getValue() const { return value; }
    float getDuration() const { return duration; }
    bool isCollected() const { return collected; }
    
    void collect();
    bool shouldDespawn() const;

private:
    void initializePowerUp();
};

#endif // POWERUP_H
