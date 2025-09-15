#include "DynamicsEngine.h"
#include "GameObject.h"

DynamicsEngine::DynamicsEngine() {
}

bool DynamicsEngine::checkObjectContact(const Collidable& objA, const Collidable& objB) {
    const GameObject* gameObjA = dynamic_cast<const GameObject*>(&objA);
    const GameObject* gameObjB = dynamic_cast<const GameObject*>(&objB);
    
    if (!gameObjA || !gameObjB) {
        return false;
    }
    
    Coordinate posA = gameObjA->getPosition();
    Coordinate posB = gameObjB->getPosition();
    Coordinate boundsA = objA.getCollisionBounds();
    Coordinate boundsB = objB.getCollisionBounds();
    
    return (posA.row < posB.row + boundsB.row &&
            posA.row + boundsA.row > posB.row &&
            posA.col < posB.col + boundsB.col &&
            posA.col + boundsA.col > posB.col);
}

bool DynamicsEngine::checkTerrainCollision(Coordinate position, Coordinate bounds, const BlockGrid& terrain) {
    for (int row = position.row; row < position.row + bounds.row; ++row) {
        for (int col = position.col; col < position.col + bounds.col; ++col) {
            if (terrain.isLocationBlocked(Coordinate(row, col))) {
                return true;
            }
        }
    }
    return false;
}

void DynamicsEngine::resolveTerrainCollision(GameObject& object, const BlockGrid& terrain) {
    Coordinate pos = object.getPosition();
    
    if (!pos.isWithinBounds()) {
        object.setPosition(pos.clampToBounds());
        return;
    }
    
    if (terrain.isLocationBlocked(pos)) {
        object.setPosition(Coordinate(1, 1));
    }
}

std::vector<Coordinate> DynamicsEngine::getAdjacentPositions(Coordinate center) const {
    std::vector<Coordinate> adjacent;
    
    for (int deltaRow = -1; deltaRow <= 1; ++deltaRow) {
        for (int deltaCol = -1; deltaCol <= 1; ++deltaCol) {
            if (deltaRow == 0 && deltaCol == 0) continue;
            
            Coordinate pos = center + Coordinate(deltaRow, deltaCol);
            if (pos.isWithinBounds()) {
                adjacent.push_back(pos);
            }
        }
    }
    
    return adjacent;
}

bool DynamicsEngine::isPositionSafe(Coordinate position, const BlockGrid& terrain) const {
    return position.isWithinBounds() && !terrain.isLocationBlocked(position);
}
