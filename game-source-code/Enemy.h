#ifndef ENEMY_H
#define ENEMY_H

#include "GameObject.h"
#include "Coordinate.h"
#include "EnemyLogic.h"

enum class EnemyType {
    RED_MONSTER = 0,
    AGGRESSIVE_MONSTER = 1,
    FAST_MONSTER = 2
};

enum class EnemyState {
    NORMAL,
    PHASING,
    STUNNED,
    FLEEING
};

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

public:
    Enemy(Coordinate startPos, EnemyType type = EnemyType::RED_MONSTER);
    
    void update() override;
    void render() override;
    
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

private:
    void updateMovement(const BlockGrid& terrain, Coordinate playerPos);
    bool canMove() const;
    void updateState();
    float getMoveCooldownForType() const;
};

#endif // ENEMY_H
