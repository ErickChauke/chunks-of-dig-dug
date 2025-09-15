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
