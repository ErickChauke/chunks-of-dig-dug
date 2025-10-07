#include "DynamicsEngine.h"
#include "GameObject.h"
#include "GameConstants.h"
#include <algorithm>

using namespace GameConstants;

DynamicsEngine::DynamicsEngine() {
}

void DynamicsEngine::handleFallingObjects(std::vector<DropItem>& items, 
                                         const BlockGrid& terrain) {
    for (auto& item : items) {
        if (item.isActive()) {
            applyGravity(item, terrain);
        }
    }
    
    items.erase(
        std::remove_if(items.begin(), items.end(),
            [](const DropItem& item) { return !item.isActive(); }),
        items.end()
    );
}

void DynamicsEngine::processAllInteractions(const std::vector<GameObject*>& entities) {
    for (size_t i = 0; i < entities.size(); ++i) {
        if (!entities[i] || !entities[i]->isActive()) continue;
        
        for (size_t j = i + 1; j < entities.size(); ++j) {
            if (!entities[j] || !entities[j]->isActive()) continue;
            
            Collidable* collidableA = dynamic_cast<Collidable*>(entities[i]);
            Collidable* collidableB = dynamic_cast<Collidable*>(entities[j]);
            
            if (collidableA && collidableB) {
                if (checkObjectContact(*collidableA, *collidableB)) {
                    collidableA->onCollision(entities[j]);
                    collidableB->onCollision(entities[i]);
                }
            }
        }
    }
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
    
    return checkAABBCollision(posA, boundsA, posB, boundsB);
}

void DynamicsEngine::applyGravity(DropItem& item, const BlockGrid& terrain) {
    Coordinate currentPos = item.getPosition();
    Coordinate newPos = currentPos + Coordinate(1, 0);
    
    if (newPos.isWithinBounds() && !terrain.isLocationBlocked(newPos)) {
        item.setPosition(newPos);
    } else {
        float currentSpeed = item.getFallSpeed();
        if (currentSpeed > 0.1f) {
            item.setFallSpeed(currentSpeed * 0.8f);
        }
    }
}

bool DynamicsEngine::checkTerrainCollision(Coordinate position, Coordinate bounds, 
                                          const BlockGrid& terrain) {
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
        Coordinate newPos = findNearestClearPosition(pos, terrain);
        object.setPosition(newPos);
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

bool DynamicsEngine::checkAABBCollision(Coordinate pos1, Coordinate bounds1,
                                       Coordinate pos2, Coordinate bounds2) const {
    return (pos1.row < pos2.row + bounds2.row &&
            pos1.row + bounds1.row > pos2.row &&
            pos1.col < pos2.col + bounds2.col &&
            pos1.col + bounds1.col > pos2.col);
}

Coordinate DynamicsEngine::findNearestClearPosition(Coordinate pos, 
                                                   const BlockGrid& terrain) const {
    Coordinate offsets[] = {{-1,0}, {1,0}, {0,-1}, {0,1}, 
                           {-1,-1}, {-1,1}, {1,-1}, {1,1}};
    
    for (const auto& offset : offsets) {
        Coordinate testPos = pos + offset;
        if (isPositionSafe(testPos, terrain)) {
            return testPos;
        }
    }
    
    return Coordinate(Coordinate::PLAYABLE_START_ROW, 1);
}
