#ifndef PARTICLESYSTEM_H
#define PARTICLESYSTEM_H

#include <raylib-cpp.hpp>
#include <vector>
#include "Coordinate.h"
#include "GameConstants.h"

/**
 * @file ParticleSystem.h
 * @brief Visual particle effects system for explosions and trails
 */

/**
 * @struct Particle
 * @brief Individual particle with physics and lifetime
 */
struct Particle {
    Vector2 position;   ///< Current screen position
    Vector2 velocity;   ///< Movement velocity (pixels/sec)
    Color color;        ///< Particle color
    float lifetime;     ///< Remaining lifetime (seconds)
    float maxLifetime;  ///< Initial lifetime for fade calculation
    float size;         ///< Particle radius (pixels)
    
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

/**
 * @class ParticleSystem
 * @brief Manages particle effects for visual feedback
 * 
 * ParticleSystem creates and updates visual particle effects:
 * - Explosion bursts (enemy destruction, rock impact)
 * - Digging trails (player movement feedback)
 * - Power-up collection sparkles
 * - Fire breath trails
 * 
 * Particle types:
 * - emit(): Radial burst (8 particles in circle pattern)
 * - emitBurst(): Large explosion (12+ particles random angles)
 * - emitTrail(): Small trail particles (2-3 particles)
 * 
 * Physics simulation:
 * - Initial velocity sets direction
 * - Gravity applies downward acceleration (200 px/s²)
 * - Lifetime determines fade and removal
 * - Size scales with alpha for smooth disappearance
 * 
 * Performance:
 * - Particles auto-removed when lifetime expires
 * - Typical count: 20-50 active particles
 * - Minimal performance impact
 * 
 * Visual feedback examples:
 * - Enemy destroyed: Yellow burst (12 particles)
 * - Rock lands: Gray burst (10 particles)
 * - Digging: Brown trail (continuous small particles)
 * - Power-up collected: Gold sparkle (12 particles)
 * 
 * @note Purely visual - no gameplay impact
 */
class ParticleSystem {
private:
    std::vector<Particle> particles;

public:
    /**
     * @brief Emit radial burst of particles
     * @param position Center position (screen coordinates)
     * @param color Particle color
     * @param count Number of particles (default: 8)
     */
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
    
    /**
     * @brief Emit large explosion burst
     * @param position Center position
     * @param color Particle color
     * @param count Number of particles (default: 12)
     */
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
    
    /**
     * @brief Emit small trail particle
     * @param position Spawn position
     * @param color Particle color
     */
    void emitTrail(Vector2 position, Color color) {
        Vector2 velocity = {
            (rand() % 60 - 30) * 0.5f,
            (rand() % 60 - 30) * 0.5f
        };
        particles.emplace_back(position, velocity, color, 0.2f, 2.0f);
    }
    
    /**
     * @brief Update all particles and remove dead ones
     */
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
    
    /**
     * @brief Render all active particles
     */
    void draw() const {
        for (const auto& particle : particles) {
            particle.draw();
        }
    }
    
    /**
     * @brief Remove all particles
     */
    void clear() {
        particles.clear();
    }
    
    /**
     * @brief Get active particle count
     * @return int Number of particles
     */
    int getParticleCount() const {
        return particles.size();
    }
};

#endif // PARTICLESYSTEM_H
