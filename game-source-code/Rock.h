#ifndef ROCK_H
#define ROCK_H

#include "GameObject.h"
#include "Coordinate.h"
#include "BlockGrid.h"

// Forward declarations
class Player;
class Enemy;

class Rock : public GameObject, public Collidable {
private:
    float fallTimer;
    float stabilityCheckTimer;
    float crushTimer;
    float lastPlayerCheckTime;
    Coordinate lastPlayerPosition;
    bool isFalling;
    bool hasLanded;
    bool playerIsMovingAway;
    
    // Constants
    const float CRUSH_DELAY = 1.0f;
    const float STATIONARY_CRUSH_DELAY = 0.3f;
    const float FALL_SPEED = 0.2f;
    const float PLAYER_MOVE_CHECK_INTERVAL = 0.1f;

public:
    Rock(Coordinate pos);
    void update() override;
    void render() override;
    
    // Physics
    void applyGravity(const BlockGrid& terrain);
    bool hasSupport(const BlockGrid& terrain) const;
    void checkStability(const BlockGrid& terrain);
    
    // Collision system
    Coordinate getCollisionBounds() const override;
    void onCollision(GameObject* other) override;
    
    // Enhanced crushing mechanics
    bool checkPlayerCrush(const Player& player);
    bool checkEnemyCrush(const Enemy& enemy) const;
    void handleCrushingLogic(const Player& player, std::vector<Enemy>& enemies);
    bool isDirectlyAbove(Coordinate target) const;
    bool isPlayerMovingAwayFromRock(const Player& player);
    
    // State queries
    bool getIsFalling() const { return isFalling; }
    bool getHasLanded() const { return hasLanded; }
    float getCrushTimeRemaining() const;

private:
    void startFalling();
    void stopFalling();
    void updatePlayerMovementTracking(const Player& player);
    float getActiveCrushDelay() const;
};

#endif // ROCK_H
