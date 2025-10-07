#ifndef DYNAMICSENGINE_H
#define DYNAMICSENGINE_H

#include "Coordinate.h"
#include "BlockGrid.h"
#include <vector>

class GameObject;
class DropItem;
class Collidable;

class DynamicsEngine {
public:
    DynamicsEngine();
    
    void handleFallingObjects(std::vector<DropItem>& items, const BlockGrid& terrain);
    void processAllInteractions(const std::vector<GameObject*>& entities);
    bool checkObjectContact(const Collidable& objA, const Collidable& objB);
    
    bool checkTerrainCollision(Coordinate position, Coordinate bounds, 
                              const BlockGrid& terrain);
    void resolveTerrainCollision(GameObject& object, const BlockGrid& terrain);
    
    std::vector<Coordinate> getAdjacentPositions(Coordinate center) const;
    bool isPositionSafe(Coordinate position, const BlockGrid& terrain) const;

private:
    void applyGravity(DropItem& item, const BlockGrid& terrain);
    bool checkAABBCollision(Coordinate pos1, Coordinate bounds1,
                           Coordinate pos2, Coordinate bounds2) const;
    Coordinate findNearestClearPosition(Coordinate pos, const BlockGrid& terrain) const;
};

#endif // DYNAMICSENGINE_H
