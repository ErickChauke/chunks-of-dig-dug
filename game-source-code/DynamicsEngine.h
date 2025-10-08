#ifndef DYNAMICSENGINE_H
#define DYNAMICSENGINE_H

#include "Coordinate.h"
#include "BlockGrid.h"
#include <vector>

class GameObject;
class DropItem;
class Collidable;

/**
 * @file DynamicsEngine.h
 * @brief Physics simulation and collision detection engine
 */

/**
 * @class DynamicsEngine
 * @brief Handles physics simulation and entity interactions
 * 
 * DynamicsEngine provides low-level physics and collision utilities:
 * - Gravity simulation for falling objects
 * - Generic collision detection (AABB)
 * - Terrain collision checking and resolution
 * - Adjacent position calculations for pathfinding
 * 
 * Physics responsibilities:
 * - Apply gravity to DropItems with deceleration
 * - Check terrain collisions for all entity types
 * - Resolve collisions by finding safe positions
 * - Process interactions between collidable entities
 * 
 * Collision detection methods:
 * - AABB (Axis-Aligned Bounding Box): Fast rectangle overlap
 * - Terrain checking: Grid-based solid block detection
 * - Safe position finding: Nearest clear coordinate
 * 
 * Design pattern:
 * - Utility class with stateless operations
 * - Takes entities as parameters (no internal state)
 * - Generic methods work on base classes
 * - Could be static but instance allows future state
 * 
 * Usage:
 * - Rock physics uses applyGravity() and terrain checking
 * - CollisionManager uses AABB checking
 * - AI pathfinding uses adjacency and safety checks
 * 
 * @note Currently used primarily for Rock gravity system
 */
class DynamicsEngine {
public:
    DynamicsEngine();
    
    /**
     * @brief Apply gravity to falling items
     * @param items DropItem vector to process
     * @param terrain Terrain for collision checking
     * @note Removes inactive items from vector
     */
    void handleFallingObjects(std::vector<DropItem>& items, 
                             const BlockGrid& terrain);
    
    /**
     * @brief Process collisions between all entities
     * @param entities GameObject pointer vector
     * @note Calls onCollision() for detected overlaps
     */
    void processAllInteractions(const std::vector<GameObject*>& entities);
    
    /**
     * @brief Check collision between two collidables
     * @param objA First collidable entity
     * @param objB Second collidable entity
     * @return true if collision detected
     */
    bool checkObjectContact(const Collidable& objA, const Collidable& objB);
    
    /**
     * @brief Check if entity collides with terrain
     * @param position Entity position
     * @param bounds Entity size
     * @param terrain Terrain to check
     * @return true if any cell blocked
     */
    bool checkTerrainCollision(Coordinate position, Coordinate bounds, 
                              const BlockGrid& terrain);
    
    /**
     * @brief Move entity away from terrain collision
     * @param object Entity to resolve
     * @param terrain Terrain to check
     * @note Moves to nearest clear position or clamps bounds
     */
    void resolveTerrainCollision(GameObject& object, const BlockGrid& terrain);
    
    /**
     * @brief Get all adjacent grid positions (8-directional)
     * @param center Center coordinate
     * @return std::vector<Coordinate> Adjacent valid positions
     * @note Excludes out-of-bounds positions
     */
    std::vector<Coordinate> getAdjacentPositions(Coordinate center) const;
    
    /**
     * @brief Check if position is clear and in bounds
     * @param position Position to check
     * @param terrain Terrain to check
     * @return true if safe for entity placement
     */
    bool isPositionSafe(Coordinate position, const BlockGrid& terrain) const;

private:
    void applyGravity(DropItem& item, const BlockGrid& terrain);
    bool checkAABBCollision(Coordinate pos1, Coordinate bounds1,
                           Coordinate pos2, Coordinate bounds2) const;
    Coordinate findNearestClearPosition(Coordinate pos, 
                                       const BlockGrid& terrain) const;
};

#endif // DYNAMICSENGINE_H
