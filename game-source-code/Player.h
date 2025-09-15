#ifndef PLAYER_H
#define PLAYER_H

#include "GameObject.h"
#include "Coordinate.h"
#include "BlockGrid.h"
#include "EnemyLogic.h"

class Player : public GameObject, public Collidable {
private:
    float moveTimer;
    float digEffectTimer;
    bool isDigging;
    Direction lastMoveDirection;
    Direction currentInputDirection;
    int tunnelsCreated;
    float moveCooldown;
    bool isMoving;
    float speedMultiplier;
    int consecutiveMoves;
    bool canDigDiagonally;

public:
    Player(Coordinate startPos = Coordinate(1, 1));
    
    void update() override;
    void render() override;
    
    bool handleMovement(BlockGrid& terrain);
    bool moveInDirection(Direction direction, BlockGrid& terrain);
    bool digTunnel(Coordinate pos, BlockGrid& terrain);
    
    Coordinate getCollisionBounds() const override;
    void onCollision(GameObject* other) override;
    
    bool getIsDigging() const;
    int getTunnelsCreated() const;
    Direction getLastMoveDirection() const;
    Direction getCurrentInputDirection() const;
    bool getIsMoving() const;
    float getSpeedMultiplier() const;
    
    void setSpeedMultiplier(float multiplier);
    void setDiagonalDigging(bool enabled);
    void reset(Coordinate newPos);

private:
    void updateMovementTimer();
    void updateDiggingEffects();
    void updateMovementStats();
    bool canMove() const;
    Direction processInputBuffer();
    float getDynamicMoveCooldown() const;
};

#endif
