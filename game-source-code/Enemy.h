#ifndef ENEMY_H
#define ENEMY_H

#include "GameObject.h"
#include "Coordinate.h"
#include "EnemyLogic.h"

/**
 * @file Enemy.h
 * @brief Enemy entities with AI behavior
 */

/**
 * @enum EnemyType
 * @brief Different enemy variants with unique behaviors
 */
enum class EnemyType {
    RED_MONSTER = 0,        ///< Basic enemy (1 HP, slow)
    AGGRESSIVE_MONSTER = 1, ///< Fast pursuer (2 HP)
    FAST_MONSTER = 2,       ///< High speed (1 HP)
    GREEN_DRAGON = 3        ///< Fire-breathing (2 HP)
};

/**
 * @enum EnemyState
 * @brief Behavioral states for enemy state machine
 */
enum class EnemyState {
    NORMAL,         ///< Standard movement and pursuit
    PHASING,        ///< Passing through solid earth
    STUNNED,        ///< Temporarily immobilized
    FLEEING,        ///< Retreating from danger
    BREATHING_FIRE  ///< Firing projectile (GREEN_DRAGON)
};

/**
 * @class Enemy
 * @brief Autonomous underground monster with AI
 * 
 * Enemies autonomously pursue player using pathfinding.
 * They can move through tunnels and solid earth (phasing).
 * 
 * Enemy types and properties:
 * - RED_MONSTER: Basic (speed 0.4s, 1 HP)
 * - AGGRESSIVE_MONSTER: Fast (speed 0.25s, 2 HP)
 * - FAST_MONSTER: Speed demon (speed 0.15s, 1 HP)
 * - GREEN_DRAGON: Fire breather (speed 0.35s, 2 HP)
 * 
 * @note Movement cooldowns vary by type
 */
class Enemy : public GameObject, public Collidable {
private:
    EnemyLogic ai;
    Direction currentDirection;
    float moveTimer;
    float moveCooldown;
    bool isPhasing;
    EnemyType enemyType;
    EnemyState currentState;
    float stateTimer;
    float baseSpeed;
    int health;
    bool isDestroyed;
    float destroyTimer;
    float destroyDuration;
    float fireBreathTimer;
    float fireBreathCooldown;
    bool canBreatheFire;

public:
    /**
     * @brief Construct enemy at position with type
     * @param startPos Initial grid coordinate
     * @param type Enemy variant (default: RED_MONSTER)
     */
    Enemy(Coordinate startPos, EnemyType type = EnemyType::RED_MONSTER);
    
    void update() override;
    void render() override;
    
    /**
     * @brief Move enemy toward target using AI
     * @param target Target coordinate (player position)
     * @param terrain Game terrain for pathfinding
     * @return true if movement successful
     */
    bool moveToward(Coordinate target, const BlockGrid& terrain);
    
    Direction getCurrentDirection() const;
    bool getIsPhasing() const;
    EnemyType getEnemyType() const;
    EnemyState getCurrentState() const;
    bool getIsDestroyed() const;
    float getDestroyProgress() const;
    
    Coordinate getCollisionBounds() const override;
    void onCollision(GameObject* other) override;
    
    void setAggressive(bool aggressive);
    void takeDamage(int damage);
    int getHealth() const;
    void destroy();
    
    bool shouldBreatheFire(Coordinate playerPos) const;
    Direction getFireDirection(Coordinate playerPos) const;

private:
    void updateMovement(const BlockGrid& terrain, Coordinate playerPos);
    bool canMove() const;
    void updateState();
    float getMoveCooldownForType() const;
    void updateFireBreathing();
};

#endif // ENEMY_H
