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
    void emit(Vector2 position, Color color, int count = 8) {
        for (int i = 0; i < count; ++i) {
            float angle = (360.0f / count * i) * DEG2RAD;
            float speed = 50.0f + (rand() % 100);
            Vector2 velocity = {
                cos(angle) * speed,
                sin(angle) * speed
            };
            particles.emplace_back(position, velocity, color, 0.5f);
        }
    }
    
    void emitBurst(Vector2 position, Color color, int count = 12) {
        for (int i = 0; i < count; ++i) {
            float angle = ((rand() % 360)) * DEG2RAD;
            float speed = 100.0f + (rand() % 150);
            Vector2 velocity = {
                cos(angle) * speed,
                sin(angle) * speed
            };
            float lifetime = 0.3f + (rand() % 100) * 0.005f;
            particles.emplace_back(position, velocity, color, lifetime, 4.0f);
        }
    }
    
    void emitTrail(Vector2 position, Color color) {
        Vector2 velocity = {
            (rand() % 60 - 30) * 0.5f,
            (rand() % 60 - 30) * 0.5f
        };
        particles.emplace_back(position, velocity, color, 0.2f, 2.0f);
    }
    
    void update() {
        for (auto& particle : particles) {
            particle.update();
        }
        
        particles.erase(
            std::remove_if(particles.begin(), particles.end(),
                [](const Particle& p) { return !p.isAlive(); }),
            particles.end()
        );
    }
    
    void draw() const {
        for (const auto& particle : particles) {
            particle.draw();
        }
    }
    
    void clear() {
        particles.clear();
    }
    
    int getParticleCount() const {
        return particles.size();
    }
};

#endif // PARTICLESYSTEM_H
