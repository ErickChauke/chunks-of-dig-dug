#ifndef ROCK_H
#define ROCK_H

#include "GameObject.h"
#include "Coordinate.h"
#include "BlockGrid.h"

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

public:
    Rock(Coordinate pos);
    void update() override;
    void render() override;
    
    void applyGravity(const BlockGrid& terrain);
    bool hasSupport(const BlockGrid& terrain) const;
    void checkStability(const BlockGrid& terrain);
    
    Coordinate getCollisionBounds() const override;
    void onCollision(GameObject* other) override;
    
    bool checkPlayerCrush(const Player& player);
    bool checkEnemyCrush(const Enemy& enemy) const;
    void handleCrushingLogic(const Player& player, std::vector<Enemy>& enemies);
    
    bool getIsFalling() const { return isFalling; }
    bool getHasLanded() const { return hasLanded; }
    float getCrushTimeRemaining() const;

private:
    void startFalling();
    void stopFalling();
    void updatePlayerMovementTracking(const Player& player);
    float getActiveCrushDelay() const;
    
    // New helper methods for better code organization
    bool isDirectHit(const Coordinate& playerPos) const;
    bool isPlayerInDangerZone(const Coordinate& playerPos) const;
    bool shouldResetCrushTimer(const Coordinate& playerPos) const;
};

#endif // ROCK_H
