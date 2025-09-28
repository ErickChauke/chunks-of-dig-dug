#ifndef DYNAMICSENGINE_H
#define DYNAMICSENGINE_H

#include "Coordinate.h"
#include "BlockGrid.h"
#include <vector>

// Forward declarations
class GameObject;
class DropItem;
class Collidable;

/**
 * @brief Manages dynamic objects and interactions
 * 
 * The DynamicsEngine handles physics simulation, collision detection,
 * and interactions between game objects.
 */
class DynamicsEngine {
public:
    /**
     * @brief Construct a new DynamicsEngine object
     */
    DynamicsEngine();
    
    /**
     * @brief Handle falling objects and gravity
     * @param items Vector of drop items to process
     * @param terrain Game terrain grid
     */
    void handleFallingObjects(std::vector<DropItem>& items, const BlockGrid& terrain);
    
    /**
     * @brief Process all object interactions
     * @param entities Vector of all game objects
     */
    void processAllInteractions(const std::vector<GameObject*>& entities);
    
    /**
     * @brief Check if two objects are in contact
     * @param objA First object
     * @param objB Second object
     * @return true if objects are colliding, false otherwise
     */
    bool checkObjectContact(const Collidable& objA, const Collidable& objB);
    
    /**
     * @brief Check if object collides with terrain
     * @param position Object position
     * @param bounds Object size
     * @param terrain Game terrain
     * @return true if collision detected, false otherwise
     */
    bool checkTerrainCollision(Coordinate position, Coordinate bounds, const BlockGrid& terrain);
    
    /**
     * @brief Resolve collision between object and terrain
     * @param object Object to resolve collision for
     * @param terrain Game terrain
     */
    void resolveTerrainCollision(GameObject& object, const BlockGrid& terrain);
    
    /**
     * @brief Get all valid adjacent positions around a coordinate
     * @param center Center coordinate
     * @return std::vector<Coordinate> List of valid adjacent positions
     */
    std::vector<Coordinate> getAdjacentPositions(Coordinate center) const;
    
    /**
     * @brief Check if a position is safe (within bounds and not blocked)
     * @param position Position to check
     * @param terrain Game terrain
     * @return true if position is safe, false otherwise
     */
    bool isPositionSafe(Coordinate position, const BlockGrid& terrain) const;

private:
    /**
     * @brief Apply gravity to a single object
     * @param item Object to apply gravity to
     * @param terrain Game terrain
     */
    void applyGravity(DropItem& item, const BlockGrid& terrain);
};

#endif // DYNAMICSENGINE_H
