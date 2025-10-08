#ifndef HARPOON_H
#define HARPOON_H

#include "GameObject.h"
#include "Coordinate.h"
#include "EnemyLogic.h"
#include <vector>

/**
 * @file Harpoon.h
 * @brief Player's extending/retracting harpoon weapon
 */

/**
 * @enum HarpoonState
 * @brief Lifecycle states for harpoon weapon
 */
enum class HarpoonState {
    EXTENDING,  ///< Growing toward target
    RETRACTING, ///< Returning to player
    IDLE        ///< Stationary (unused)
};

class Player; // Forward declaration

/**
 * @class Harpoon
 * @brief Extending projectile weapon that maintains player connection
 * 
 * The Harpoon is a segmented projectile that:
 * - Extends in one direction (UP/DOWN/LEFT/RIGHT)
 * - Maintains visual connection to moving player
 * - Retracts after hitting enemy or reaching max range
 * - Updates connection point every frame
 * 
 * Technical details:
 * - Max range: 3-4 cells
 * - Speed: 4-5 cells/second
 * - Segments: Vector of coordinates forming line
 * - Player tracking: Non-owning Player* reference
 * 
 * Lifecycle:
 * 1. EXTENDING: Growing toward target
 * 2. Hit enemy OR max range → RETRACTING
 * 3. Fully retracted → setActive(false)
 * 
 * @note Harpoon maintains Player* reference for connection
 */
class Harpoon : public GameObject {
private:
    Direction direction;
    float speed;
    float maxRange;
    float currentLength;
    HarpoonState state;
    Coordinate startPosition;
    Player* playerRef;
    std::vector<Coordinate> segments;

public:
    /**
     * @brief Construct harpoon fired by player
     * @param startPos Initial position (player location)
     * @param dir Firing direction
     * @param player Pointer to player for connection tracking
     */
    Harpoon(Coordinate startPos, Direction dir, Player* player);
    
    void update() override;
    void render() override;
    
    /**
     * @brief Check if harpoon hit enemy at position
     * @param enemyPos Enemy coordinate to check
     * @return true if any segment overlaps enemy
     */
    bool checkEnemyHit(Coordinate enemyPos);
    
    Direction getDirection() const;
    HarpoonState getState() const;
    const std::vector<Coordinate>& getSegments() const;
    
    /**
     * @brief Update base position to track moving player
     * @note Called every frame to maintain connection
     */
    void updatePlayerConnection();

private:
    void extend();
    void retract();
    void recalculateSegments();
};

#endif // HARPOON_H
