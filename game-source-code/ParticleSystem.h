#ifndef PARTICLESYSTEM_H
#define PARTICLESYSTEM_H

#include <raylib-cpp.hpp>
#include <vector>
#include "Coordinate.h"
#include "GameConstants.h"

struct Particle {
    Vector2 position;
    Vector2 velocity;
    Color color;
    float lifetime;
    float maxLifetime;
    float size;
    
    Particle(Vector2 pos, Vector2 vel, Color col, float life, float sz = 3.0f)
        : position(pos), velocity(vel), color(col), 
          lifetime(life), maxLifetime(life), size(sz) {}
    
    void update() {
        position.x += velocity.x * GetFrameTime();
        position.y += velocity.y * GetFrameTime();
        velocity.y += 200.0f * GetFrameTime();
        lifetime -= GetFrameTime();
    }
    
    void draw() const {
        float alpha = lifetime / maxLifetime;
        Color drawColor = ColorAlpha(color, alpha);
        DrawCircleV(position, size * alpha, drawColor);
    }
    
    bool isAlive() const {
        return lifetime > 0.0f;
    }
};

class ParticleSystem {
private:
    std::vector<Particle> particles;

public:
    void update();
    void draw() const;
    void clear();
    int getParticleCount() const;
};

#endif // PARTICLESYSTEM_H
