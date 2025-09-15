#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "Coordinate.h"

class GameObject {
protected:
    Coordinate position;
    bool active;

public:
    GameObject(Coordinate pos = Coordinate(0, 0));
    virtual ~GameObject() = default;
    
    virtual void update() = 0;
    virtual void render() = 0;
    
    Coordinate getPosition() const;
    void setPosition(Coordinate pos);
    bool isActive() const;
    void setActive(bool state);
};

class Collidable {
public:
    virtual ~Collidable() = default;
    virtual Coordinate getCollisionBounds() const = 0;
    virtual void onCollision(GameObject* other) = 0;
};

#endif
