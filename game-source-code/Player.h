#ifndef PLAYER_H
#define PLAYER_H

#include "GameObject.h"
#include "Coordinate.h"
#include "BlockGrid.h"
#include "EnemyLogic.h"

class Player : public GameObject, public Collidable {
private:
    float moveTimer;
    bool isDigging;
    Direction lastMoveDirection;
    int tunnelsCreated;
    bool isMoving;
    float moveCooldown;

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
    bool getIsMoving() const;
    void reset(Coordinate newPos);

private:
    bool canMove() const;
};

#endif
