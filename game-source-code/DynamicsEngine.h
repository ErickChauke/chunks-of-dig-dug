#ifndef DYNAMICSENGINE_H
#define DYNAMICSENGINE_H

#include "Coordinate.h"
#include "BlockGrid.h"
#include <vector>

class GameObject;
class Collidable;

class DynamicsEngine {
public:
    DynamicsEngine();
    
    bool checkObjectContact(const Collidable& objA, const Collidable& objB);
    bool checkTerrainCollision(Coordinate position, Coordinate bounds, const BlockGrid& terrain);
    void resolveTerrainCollision(GameObject& object, const BlockGrid& terrain);
    std::vector<Coordinate> getAdjacentPositions(Coordinate center) const;
    bool isPositionSafe(Coordinate position, const BlockGrid& terrain) const;
};

#endif
