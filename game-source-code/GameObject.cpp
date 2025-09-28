#include "GameObject.h"

GameObject::GameObject(Coordinate pos) : position(pos), active(true) {
}

Coordinate GameObject::getPosition() const {
    return position;
}

void GameObject::setPosition(Coordinate pos) {
    position = pos;
}

bool GameObject::isActive() const {
    return active;
}

void GameObject::setActive(bool state) {
    active = state;
}

// DropItem implementation
DropItem::DropItem(Coordinate pos, int type) 
    : GameObject(pos), fallSpeed(1.0f), itemType(type) {
}

void DropItem::update() {
    // Basic falling behavior - will be enhanced in physics sprint
    if (active) {
        position.row += static_cast<int>(fallSpeed);
    }
}

void DropItem::render() {
    // Placeholder rendering - will be implemented with graphics
}

Coordinate DropItem::getCollisionBounds() const {
    return Coordinate(1, 1); // Single cell collision
}

void DropItem::onCollision(GameObject* other) {
    // Handle collision - to be implemented
}

float DropItem::getFallSpeed() const {
    return fallSpeed;
}

void DropItem::setFallSpeed(float speed) {
    fallSpeed = speed;
}
