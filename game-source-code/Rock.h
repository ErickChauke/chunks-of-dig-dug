#ifndef ROCK_H
#define ROCK_H

#include "GameObject.h"
#include "Coordinate.h"
#include "BlockGrid.h"

class Player;
class Enemy;

/**
 * @file Rock.h
 * @brief Falling rock with gravity and context-aware crushing
 */

/**
 * @class Rock
 * @brief Physics-based falling rock obstacle
 * 
 * Rocks implement realistic falling physics with context-aware
 * crushing mechanics that adapt to player behavior:
 * 
 * Crushing delays (context-aware):
 * - Moving player: 1.2 seconds (time to escape)
 * - Stationary player: 0.4 seconds (penalty for freezing)
 * 
 * Physics system:
 * - Gravity: 0.18s fall interval per cell
 * - Support checking: Every 0.1s
 * - Falls when support removed (player digs underneath)
 * - Stops when hits ground or earth
 * 
 * Crushing logic:
 * - Direct hit: Instant crush
 * - Danger zone: Player directly below falling rock
 * - Tracks player movement every 0.1s
 * - Adjusts delay based on movement state
 * 
 * @note Context-aware delays create risk/reward decisions
 */
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
    /**
     * @brief Construct rock at position
     * @param pos Initial grid coordinate
     */
    Rock(Coordinate pos);
    
    void update() override;
    void render() override;
    
    /**
     * @brief Apply gravity physics
     * @param terrain Game terrain for collision checking
     */
    void applyGravity(const BlockGrid& terrain);
    
    /**
     * @brief Check if rock has support below
     * @param terrain Game terrain to check
     * @return true if supported
     */
    bool hasSupport(const BlockGrid& terrain) const;
    
    /**
     * @brief Check stability and update falling state
     * @param terrain Game terrain
     */
    void checkStability(const BlockGrid& terrain);
    
    Coordinate getCollisionBounds() const override;
    void onCollision(GameObject* other) override;
    
    /**
     * @brief Check if rock should crush player
     * @param player Player to check
     * @return true if crush conditions met
     */
    bool checkPlayerCrush(const Player& player);
    
    /**
     * @brief Check if rock should crush enemy
     * @param enemy Enemy to check
     * @return true if rock overlaps enemy
     */
    bool checkEnemyCrush(const Enemy& enemy) const;
    
    /**
     * @brief Handle all crushing logic for frame
     * @param player Player reference
     * @param enemies Enemy vector (modified if crushed)
     */
    void handleCrushingLogic(const Player& player, std::vector<Enemy>& enemies);
    
    bool getIsFalling() const { return isFalling; }
    bool getHasLanded() const { return hasLanded; }
    float getCrushTimeRemaining() const;

private:
    void startFalling();
    void stopFalling();
    void updatePlayerMovementTracking(const Player& player);
    float getActiveCrushDelay() const;
    bool isDirectHit(const Coordinate& playerPos) const;
    bool isPlayerInDangerZone(const Coordinate& playerPos) const;
    bool shouldResetCrushTimer(const Coordinate& playerPos) const;
};

#endif // ROCK_H
