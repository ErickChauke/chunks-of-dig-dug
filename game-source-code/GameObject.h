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
    
    /**
     * @brief Update object state
     */
    virtual void update() = 0;
    
    /**
     * @brief Render object to screen
     */
    virtual void render() = 0;
    
    /**
     * @brief Get current position
     * @return Coordinate Current position
     */
    Coordinate getPosition() const;
    
    /**
     * @brief Set object position
     * @param pos New position
     */
    void setPosition(Coordinate pos);
    
    /**
     * @brief Check if object is active
     * @return true if active, false otherwise
     */
    bool isActive() const;
    
    /**
     * @brief Set object active state
     * @param state New active state
     */
    void setActive(bool state);
};

/**
 * @brief Interface for objects that can collide
 */
class Collidable {
public:
    virtual ~Collidable() = default;
    
    /**
     * @brief Get collision bounds
     * @return Coordinate Size of collision area
     */
    virtual Coordinate getCollisionBounds() const = 0;
    
    /**
     * @brief Handle collision with another object
     * @param other Object that collided with this one
     */
    virtual void onCollision(GameObject* other) = 0;
};

/**
 * @brief Represents falling items in the game
 */
class DropItem : public GameObject, public Collidable {
private:
    float fallSpeed;
    int itemType;

public:
    /**
     * @brief Construct a new DropItem
     * @param pos Initial position
     * @param type Item type identifier
     */
    DropItem(Coordinate pos, int type);
    
    void update() override;
    void render() override;
    Coordinate getCollisionBounds() const override;
    void onCollision(GameObject* other) override;
    
    /**
     * @brief Get fall speed
     * @return float Current fall speed
     */
    float getFallSpeed() const;
    
    /**
     * @brief Set fall speed
     * @param speed New fall speed
     */
    void setFallSpeed(float speed);
};

#endif // GAMEOBJECT_H
