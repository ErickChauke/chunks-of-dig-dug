#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "Coordinate.h"

/**
 * @brief Base class for all game objects
 * 
 * GameObject provides the foundation for all interactive elements
 * in the game including players, enemies, and items.
 */
class GameObject {
protected:
    Coordinate position;
    bool active;

public:
    /**
     * @brief Construct a new GameObject
     * @param pos Initial position
     */
    GameObject(Coordinate pos = Coordinate(0, 0));
    
    /**
     * @brief Virtual destructor for proper inheritance
     */
    virtual ~GameObject() = default;
    
    virtual void update() = 0;
    virtual void render() = 0;
    
    Coordinate getPosition() const;
    void setPosition(Coordinate pos);
    bool isActive() const;
    void setActive(bool state);
};

/**
 * @brief Interface for objects that can collide
 */
class Collidable {
public:
    virtual ~Collidable() = default;
    virtual Coordinate getCollisionBounds() const = 0;
    virtual void onCollision(GameObject* other) = 0;
};

class DropItem : public GameObject, public Collidable {
private:
    float fallSpeed;
    int itemType;

public:
    DropItem(Coordinate pos, int type);
    
    void update() override;
    void render() override;
    Coordinate getCollisionBounds() const override;
    void onCollision(GameObject* other) override;
    
    float getFallSpeed() const;
    void setFallSpeed(float speed);
};

#endif
