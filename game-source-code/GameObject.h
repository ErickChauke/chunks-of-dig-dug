#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "Coordinate.h"

/**
 * @file GameObject.h
 * @brief Base classes for all game entities and collision system
 */

/**
 * @class GameObject
 * @brief Abstract base class for all interactive game entities
 * 
 * GameObject provides the foundation for the game's entity system.
 * All moving objects (Player, Enemy, Harpoon, Rock, PowerUp) inherit
 * from this class to gain position tracking and lifecycle management.
 * 
 * The class enforces the contract that all entities must implement
 * update() and render() methods, supporting polymorphic game loops.
 * 
 * @note This follows the Entity pattern from game engine architecture
 */
class GameObject {
protected:
    Coordinate position; ///< Current grid position of entity
    bool active;         ///< Lifecycle state (true=alive, false=destroyed)

public:
    /**
     * @brief Construct GameObject at specified position
     * @param pos Initial grid coordinate (default: origin)
     */
    GameObject(Coordinate pos = Coordinate(0, 0));
    
    /**
     * @brief Virtual destructor for proper inheritance cleanup
     * @note Ensures derived class destructors are called correctly
     */
    virtual ~GameObject() = default;
    
    /**
     * @brief Update entity state for current frame
     * @note Called every frame by game loop. Must be implemented by derived classes.
     */
    virtual void update() = 0;
    
    /**
     * @brief Render entity to screen
     * @note Called every frame after update(). Must be implemented by derived classes.
     */
    virtual void render() = 0;
    
    /**
     * @brief Get current position
     * @return Coordinate Current grid coordinate
     */
    Coordinate getPosition() const;
    
    /**
     * @brief Set entity position
     * @param pos New grid coordinate
     * @note Does not perform bounds checking - caller's responsibility
     */
    void setPosition(Coordinate pos);
    
    /**
     * @brief Check if entity is active
     * @return true if active (alive), false if inactive (destroyed)
     */
    bool isActive() const;
    
    /**
     * @brief Set entity active state
     * @param state New active state (true=activate, false=deactivate)
     * @note Setting to false marks entity for removal
     */
    void setActive(bool state);
};

/**
 * @interface Collidable
 * @brief Interface for entities that participate in collision detection
 * 
 * The Collidable interface enables generic collision detection through
 * polymorphism. Any entity implementing this interface can be checked
 * for collisions by the CollisionManager without knowing specific types.
 * 
 * Design pattern: Interface Segregation Principle (ISP)
 * 
 * @note Entities should implement both GameObject and Collidable
 */
class Collidable {
public:
    /**
     * @brief Virtual destructor for interface
     */
    virtual ~Collidable() = default;
    
    /**
     * @brief Get collision bounding box size
     * @return Coordinate Size of collision area (width, height in cells)
     * @note Used for AABB (Axis-Aligned Bounding Box) collision detection
     */
    virtual Coordinate getCollisionBounds() const = 0;
    
    /**
     * @brief Handle collision event with another entity
     * @param other Entity that collided with this one
     * @note Called by CollisionManager when collision detected
     */
    virtual void onCollision(GameObject* other) = 0;
};

/**
 * @class DropItem
 * @brief Represents falling collectible items affected by gravity
 * 
 * DropItem extends GameObject with physics-based falling behavior.
 * Used for bonus items that appear and fall until collected or landing.
 * 
 * @note Currently a placeholder - full implementation in future sprints
 */
class DropItem : public GameObject, public Collidable {
private:
    float fallSpeed; ///< Vertical falling speed (cells per second)
    int itemType;    ///< Item type identifier

public:
    /**
     * @brief Construct falling item at position
     * @param pos Initial grid coordinate
     * @param type Item type identifier (0=default)
     */
    DropItem(Coordinate pos, int type);
    
    void update() override;
    void render() override;
    Coordinate getCollisionBounds() const override;
    void onCollision(GameObject* other) override;
    
    /**
     * @brief Get current falling speed
     * @return float Speed in cells per second
     */
    float getFallSpeed() const;
    
    /**
     * @brief Set falling speed
     * @param speed New speed in cells per second
     * @note Used to implement gravity acceleration
     */
    void setFallSpeed(float speed);
};

#endif // GAMEOBJECT_H
