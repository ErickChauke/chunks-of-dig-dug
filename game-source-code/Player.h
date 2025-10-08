#ifndef PLAYER_H
#define PLAYER_H

#include "GameObject.h"
#include "Coordinate.h"
#include "BlockGrid.h"
#include "EnemyLogic.h"
#include <vector>

class Rock; // Forward declaration

/**
 * @file Player.h
 * @brief Player character with movement, tunneling, and state management
 */

/**
 * @class Player
 * @brief Represents the player-controlled character (Dig Dug)
 * 
 * The Player class handles grid-based 4-directional movement,
 * tunnel digging through solid earth blocks, movement cooldown
 * system with dynamic speed adjustments, and collision detection.
 * 
 * Movement mechanics:
 * - Base cooldown: 0.12 seconds between moves
 * - Speed boost: Modified by power-ups (0.5x = twice as fast)
 * - Consecutive moves: Slight speed increase after multiple moves
 * - Digging: Automatically clears earth blocks when moving
 * 
 * @note Player is both GameObject and Collidable
 */
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
    /**
     * @brief Construct player at starting position
     * @param startPos Initial grid coordinate (default: 1,1)
     */
    Player(Coordinate startPos = Coordinate(1, 1));
    
    void update() override;
    void render() override;
    
    /**
     * @brief Process player movement without rock checking
     * @param terrain Game terrain to check for earth blocks
     * @return true if movement successful
     */
    bool handleMovement(BlockGrid& terrain);
    
    /**
     * @brief Process player movement with rock avoidance
     * @param terrain Game terrain to check
     * @param rocks Active rocks to check for collisions
     * @return true if movement successful
     */
    bool handleMovementWithRocks(BlockGrid& terrain, const std::vector<Rock>& rocks);
    
    /**
     * @brief Move player in specified direction
     * @param direction Direction to move (UP/DOWN/LEFT/RIGHT)
     * @param terrain Game terrain for checking
     * @return true if movement successful
     */
    bool moveInDirection(Direction direction, BlockGrid& terrain);
    
    /**
     * @brief Move player with rock collision avoidance
     * @param direction Direction to move
     * @param terrain Game terrain
     * @param rocks Active rocks to avoid
     * @return true if movement successful
     */
    bool moveInDirectionWithRocks(Direction direction, BlockGrid& terrain, 
                                 const std::vector<Rock>& rocks);
    
    /**
     * @brief Dig tunnel through earth block
     * @param pos Coordinate of earth block to clear
     * @param terrain Game terrain to modify
     * @return true if tunnel created
     */
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
    
    Coordinate getDirectionOffset(Direction direction) const;
    void updateDirectionState(Direction direction);
    void updateMovementState(bool moved);
};

#endif // PLAYER_H
