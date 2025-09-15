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

DropItem::DropItem(Coordinate pos, int type) 
    : GameObject(pos), fallSpeed(1.0f), itemType(type) {
}

void DropItem::update() {
    if (active) {
        position.row += static_cast<int>(fallSpeed);
    }
}

void DropItem::render() {
}

Coordinate DropItem::getCollisionBounds() const {
    return Coordinate(1, 1);
}

void DropItem::onCollision(GameObject* other) {
}

float DropItem::getFallSpeed() const {
    return fallSpeed;
}

void DropItem::setFallSpeed(float speed) {
    fallSpeed = speed;
}
