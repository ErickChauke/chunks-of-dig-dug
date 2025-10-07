#include "Enemy.h"
#include "BlockGrid.h"
#include "GameConstants.h"
#include <raylib-cpp.hpp>

using namespace GameConstants;

const float DESTROY_DURATION = 0.8f;
const float FIRE_BREATH_COOLDOWN = 2.5f;
const float FIRE_BREATH_STATE_DURATION = 0.5f;

Enemy::Enemy(Coordinate startPos, EnemyType type) 
    : GameObject(startPos), enemyType(type), currentDirection(Direction::NONE),
      moveTimer(0.0f), isPhasing(false), currentState(EnemyState::NORMAL),
      stateTimer(0.0f), baseSpeed(1.0f), health(1), isDestroyed(false),
      destroyTimer(0.0f), destroyDuration(DESTROY_DURATION),
      fireBreathTimer(FIRE_BREATH_COOLDOWN), fireBreathCooldown(FIRE_BREATH_COOLDOWN),
      canBreatheFire(false) {
    
    moveCooldown = getMoveCooldownForType();
    
    if (type == EnemyType::AGGRESSIVE_MONSTER) {
        ai.setAggressive(true);
        health = 2;
    } else if (type == EnemyType::GREEN_DRAGON) {
        health = 2;
        canBreatheFire = true;
    }
    
    float randomFactor = 0.8f + (std::rand() % 40) * 0.01f;
    moveCooldown *= randomFactor;
}

void Enemy::update() {
    if (isDestroyed) {
        destroyTimer += GetFrameTime();
        if (destroyTimer >= destroyDuration) {
            setActive(false);
        }
        return;
    }
    
    updateState();
    updateFireBreathing();
}

void Enemy::render() {
}

bool Enemy::moveToward(Coordinate target, const BlockGrid& terrain) {
    if (!canMove() || isDestroyed) {
        return false;
    }
    
    Direction nextMove = ai.selectNextAction(position, target, terrain);
    
    if (nextMove == Direction::NONE) {
        return false;
    }
    
    Coordinate offset;
    switch (nextMove) {
        case Direction::UP:    offset = Coordinate(-1, 0); break;
        case Direction::DOWN:  offset = Coordinate(1, 0); break;
        case Direction::LEFT:  offset = Coordinate(0, -1); break;
        case Direction::RIGHT: offset = Coordinate(0, 1); break;
        case Direction::NONE:  return false;
    }
    
    Coordinate newPos = position + offset;
    
    if (!newPos.isWithinBounds()) {
        return false;
    }
    
    if (terrain.isLocationBlocked(newPos)) {
        isPhasing = true;
        currentState = EnemyState::PHASING;
        stateTimer = GetTime();
    } else {
        isPhasing = false;
        if (currentState == EnemyState::PHASING) {
            currentState = EnemyState::NORMAL;
        }
    }
    
    position = newPos;
    currentDirection = nextMove;
    moveTimer = GetTime();
    return true;
}

Direction Enemy::getCurrentDirection() const {
    return currentDirection;
}

bool Enemy::getIsPhasing() const {
    return isPhasing;
}

EnemyType Enemy::getEnemyType() const {
    return enemyType;
}

EnemyState Enemy::getCurrentState() const {
    return currentState;
}

bool Enemy::getIsDestroyed() const {
    return isDestroyed;
}

float Enemy::getDestroyProgress() const {
    if (!isDestroyed) return 0.0f;
    return destroyTimer / destroyDuration;
}

Coordinate Enemy::getCollisionBounds() const {
    return Coordinate(1, 1);
}

void Enemy::onCollision(GameObject* other) {
    if (other) {
    }
}

void Enemy::setAggressive(bool aggressive) {
    ai.setAggressive(aggressive);
    if (aggressive && enemyType == EnemyType::RED_MONSTER) {
        enemyType = EnemyType::AGGRESSIVE_MONSTER;
        moveCooldown = getMoveCooldownForType();
    }
}

void Enemy::takeDamage(int damage) {
    if (isDestroyed) return;
    
    health -= damage;
    if (health <= 0) {
        destroy();
    } else {
        currentState = EnemyState::STUNNED;
        stateTimer = GetTime();
    }
}

int Enemy::getHealth() const {
    return health;
}

void Enemy::destroy() {
    isDestroyed = true;
    destroyTimer = 0.0f;
}

void Enemy::updateMovement(const BlockGrid& terrain, Coordinate playerPos) {
    if (!isDestroyed) {
        moveToward(playerPos, terrain);
    }
}

bool Enemy::canMove() const {
    if (isDestroyed) return false;
    
    if (currentState == EnemyState::STUNNED) {
        return (GetTime() - stateTimer) > 0.5f;
    }
    
    if (currentState == EnemyState::BREATHING_FIRE) {
        return false;
    }
    
    float currentTime = GetTime();
    return (currentTime - moveTimer) >= moveCooldown;
}

void Enemy::updateState() {
    float currentTime = GetTime();
    
    switch (currentState) {
        case EnemyState::STUNNED:
            if (currentTime - stateTimer > 0.5f) {
                currentState = EnemyState::NORMAL;
            }
            break;
        case EnemyState::PHASING:
            if (!isPhasing) {
                currentState = EnemyState::NORMAL;
            }
            break;
        case EnemyState::BREATHING_FIRE:
            if (currentTime - stateTimer > FIRE_BREATH_STATE_DURATION) {
                currentState = EnemyState::NORMAL;
                fireBreathTimer = 0.0f;
            }
            break;
        case EnemyState::NORMAL:
        case EnemyState::FLEEING:
            break;
    }
}

float Enemy::getMoveCooldownForType() const {
    switch (enemyType) {
        case EnemyType::RED_MONSTER:       return 0.4f;
        case EnemyType::AGGRESSIVE_MONSTER: return 0.25f;
        case EnemyType::FAST_MONSTER:      return 0.15f;
        case EnemyType::GREEN_DRAGON:      return 0.35f;
    }
    return 0.4f;
}

void Enemy::updateFireBreathing() {
    if (!canBreatheFire) return;
    
    if (currentState != EnemyState::BREATHING_FIRE) {
        fireBreathTimer += GetFrameTime();
    }
}

bool Enemy::shouldBreatheFire(Coordinate playerPos) const {
    if (!canBreatheFire || isDestroyed) return false;
    if (currentState == EnemyState::BREATHING_FIRE) return false;
    if (currentState == EnemyState::STUNNED) return false;
    if (fireBreathTimer < fireBreathCooldown) return false;
    
    float distance = position.calculateDistance(playerPos);
    bool inRange = distance < 8.0f && distance > 1.0f;
    
    if (inRange) {
        const_cast<Enemy*>(this)->currentState = EnemyState::BREATHING_FIRE;
        const_cast<Enemy*>(this)->stateTimer = GetTime();
        return true;
    }
    
    return false;
}

Direction Enemy::getFireDirection(Coordinate playerPos) const {
    int deltaRow = playerPos.row - position.row;
    int deltaCol = playerPos.col - position.col;
    
    if (std::abs(deltaRow) > std::abs(deltaCol)) {
        return (deltaRow > 0) ? Direction::DOWN : Direction::UP;
    } else {
        return (deltaCol > 0) ? Direction::RIGHT : Direction::LEFT;
    }
}
