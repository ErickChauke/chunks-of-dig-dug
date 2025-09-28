#include "DynamicsEngine.h"
#include "GameObject.h"
#include <algorithm>

DynamicsEngine::DynamicsEngine() {
}

void DynamicsEngine::handleFallingObjects(std::vector<DropItem>& items, const BlockGrid& terrain) {
    for (auto& item : items) {
        if (item.isActive()) {
            applyGravity(item, terrain);
        }
    }
    
    // Remove inactive items
    items.erase(
        std::remove_if(items.begin(), items.end(),
            [](const DropItem& item) { return !item.isActive(); }),
        items.end()
    );
}

void DynamicsEngine::processAllInteractions(const std::vector<GameObject*>& entities) {
    // Check collisions between all pairs of entities
    for (size_t i = 0; i < entities.size(); ++i) {
        if (!entities[i] || !entities[i]->isActive()) continue;
        
        for (size_t j = i + 1; j < entities.size(); ++j) {
            if (!entities[j] || !entities[j]->isActive()) continue;
            
            // Try to cast to Collidable interface
            Collidable* collidableA = dynamic_cast<Collidable*>(entities[i]);
            Collidable* collidableB = dynamic_cast<Collidable*>(entities[j]);
            
            if (collidableA && collidableB) {
                if (checkObjectContact(*collidableA, *collidableB)) {
                    // Handle collision
                    collidableA->onCollision(entities[j]);
                    collidableB->onCollision(entities[i]);
                }
            }
        }
    }
}

bool DynamicsEngine::checkObjectContact(const Collidable& objA, const Collidable& objB) {
    // Get object positions (assuming they inherit from GameObject)
    const GameObject* gameObjA = dynamic_cast<const GameObject*>(&objA);
    const GameObject* gameObjB = dynamic_cast<const GameObject*>(&objB);
    
    if (!gameObjA || !gameObjB) {
        return false; // Can't check collision without position info
    }
    
    Coordinate posA = gameObjA->getPosition();
    Coordinate posB = gameObjB->getPosition();
    Coordinate boundsA = objA.getCollisionBounds();
    Coordinate boundsB = objB.getCollisionBounds();
    
    // Simple AABB (Axis-Aligned Bounding Box) collision detection
    return (posA.row < posB.row + boundsB.row &&
            posA.row + boundsA.row > posB.row &&
            posA.col < posB.col + boundsB.col &&
            posA.col + boundsA.col > posB.col);
}

void DynamicsEngine::applyGravity(DropItem& item, const BlockGrid& terrain) {
    Coordinate currentPos = item.getPosition();
    Coordinate newPos = currentPos + Coordinate(1, 0); // Move down one row
    
    // Check if new position is valid and not blocked
    if (newPos.isWithinBounds() && !terrain.isLocationBlocked(newPos)) {
        item.setPosition(newPos);
    } else {
        // Item has landed - could trigger landing effects here
        // For now, just slow down the fall speed
        float currentSpeed = item.getFallSpeed();
        if (currentSpeed > 0.1f) {
            item.setFallSpeed(currentSpeed * 0.8f); // Gradual stop
        }
    }
}

bool DynamicsEngine::checkTerrainCollision(Coordinate position, Coordinate bounds, const BlockGrid& terrain) {
    // Check if any part of the object overlaps with blocked terrain
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
    
    // Simple resolution: move object to nearest valid position
    if (!pos.isWithinBounds()) {
        object.setPosition(pos.clampToBounds());
        return;
    }
    
    // If current position is blocked, try to find nearest clear position
    if (terrain.isLocationBlocked(pos)) {
        // Try adjacent positions
        Coordinate offsets[] = {{-1,0}, {1,0}, {0,-1}, {0,1}, {-1,-1}, {-1,1}, {1,-1}, {1,1}};
        
        for (const auto& offset : offsets) {
            Coordinate testPos = pos + offset;
            if (testPos.isWithinBounds() && !terrain.isLocationBlocked(testPos)) {
                object.setPosition(testPos);
                return;
            }
        }
        
        // If no adjacent position works, move to a safe default position
        object.setPosition(Coordinate(1, 1));
    }
}

std::vector<Coordinate> DynamicsEngine::getAdjacentPositions(Coordinate center) const {
    std::vector<Coordinate> adjacent;
    
    // Check 8 directions around center
    for (int deltaRow = -1; deltaRow <= 1; ++deltaRow) {
        for (int deltaCol = -1; deltaCol <= 1; ++deltaCol) {
            if (deltaRow == 0 && deltaCol == 0) continue; // Skip center
            
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
