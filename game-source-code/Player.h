#ifndef PLAYER_H
#define PLAYER_H

#include "GameObject.h"
#include "Coordinate.h"
#include "BlockGrid.h"
#include "EnemyLogic.h"
#include <vector>

// Forward declaration
class Rock;

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
    bool hasHarpoon;
    float harpoonCooldown;
    float lastHarpoonTime;

public:
    Player(Coordinate startPos = Coordinate(1, 1));
    
    void update() override;
    void render() override;
    
    bool handleMovement(BlockGrid& terrain);
    bool handleMovementWithRocks(BlockGrid& terrain, const std::vector<Rock>& rocks);
    bool moveInDirection(Direction direction, BlockGrid& terrain);
    bool moveInDirectionWithRocks(Direction direction, BlockGrid& terrain, 
                                 const std::vector<Rock>& rocks);
    bool digTunnel(Coordinate pos, BlockGrid& terrain);
    
    Coordinate getCollisionBounds() const override;
    void onCollision(GameObject* other) override;
    
    bool getIsDigging() const;
    int getTunnelsCreated() const;
    Direction getLastMoveDirection() const;
    Direction getCurrentInputDirection() const;
    bool getIsMoving() const;
    float getSpeedMultiplier() const;
    bool canFireHarpoon() const;
    
    Coordinate& getPositionRef() { return position; }
    
    void setSpeedMultiplier(float multiplier);
    void setDiagonalDigging(bool enabled);
    void reset(Coordinate newPos);

private:
    void updateMovementTimer();
    void updateDiggingEffects();
    bool canMove() const;
    Direction processInputBuffer();
    void updateMovementStats();
    float getDynamicMoveCooldown() const;
    bool isPositionBlockedByRock(Coordinate pos, const std::vector<Rock>& rocks) const;
};

#endif // PLAYER_H
