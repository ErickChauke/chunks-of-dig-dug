#ifndef FIREPROJECTILE_H
#define FIREPROJECTILE_H

#include "GameObject.h"
#include "Coordinate.h"
#include "EnemyLogic.h"
#include "GameConstants.h"

class FireProjectile : public GameObject, public Collidable {
private:
    Direction direction;
    float speed;
    float lifetime;
    float maxLifetime;
    std::vector<Coordinate> trail;
    
public:
    FireProjectile(Coordinate startPos, Direction dir) 
        : GameObject(startPos), direction(dir), speed(3.0f), 
          lifetime(2.0f), maxLifetime(2.0f) {
        trail.push_back(startPos);
    }
    
    void update() override {
        if (!active) return;
        
        lifetime -= GetFrameTime();
        if (lifetime <= 0.0f) {
            setActive(false);
            return;
        }
        
        static float moveTimer = 0.0f;
        moveTimer += GetFrameTime();
        
        if (moveTimer >= (1.0f / speed)) {
            Coordinate offset;
            switch (direction) {
                case Direction::UP:    offset = Coordinate(-1, 0); break;
                case Direction::DOWN:  offset = Coordinate(1, 0); break;
                case Direction::LEFT:  offset = Coordinate(0, -1); break;
                case Direction::RIGHT: offset = Coordinate(0, 1); break;
                case Direction::NONE:  return;
            }
            
            Coordinate newPos = position + offset;
            
            if (!newPos.isWithinBounds()) {
                setActive(false);
                return;
            }
            
            position = newPos;
            trail.push_back(newPos);
            
            if (trail.size() > 5) {
                trail.erase(trail.begin());
            }
            
            moveTimer = 0.0f;
        }
    }
    
    void render() override {
    }
    
    Coordinate getCollisionBounds() const override {
        return Coordinate(1, 1);
    }
    
    void onCollision(GameObject* other) override {
        if (other && other->isActive()) {
            setActive(false);
        }
    }
    
    Direction getDirection() const {
        return direction;
    }
    
    const std::vector<Coordinate>& getTrail() const {
        return trail;
    }
    
    float getLifetimeRatio() const {
        return lifetime / maxLifetime;
    }
    
    bool checkPlayerHit(Coordinate playerPos) const {
        if (!active) return false;
        return position == playerPos;
    }
};

#endif // FIREPROJECTILE_H
