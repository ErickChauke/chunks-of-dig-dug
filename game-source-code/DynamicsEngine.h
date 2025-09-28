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
 */
class DynamicsEngine {
public:
    DynamicsEngine();
    
    void handleFallingObjects(std::vector<DropItem>& items, const BlockGrid& terrain);
    void processAllInteractions(const std::vector<GameObject*>& entities);
    bool checkObjectContact(const Collidable& objA, const Collidable& objB);
    bool checkTerrainCollision(Coordinate position, Coordinate bounds, const BlockGrid& terrain);
    void resolveTerrainCollision(GameObject& object, const BlockGrid& terrain);
    std::vector<Coordinate> getAdjacentPositions(Coordinate center) const;
    bool isPositionSafe(Coordinate position, const BlockGrid& terrain) const;

private:
    void applyGravity(DropItem& item, const BlockGrid& terrain);
};

#endif // DYNAMICSENGINE_H
