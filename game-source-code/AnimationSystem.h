#ifndef ANIMATIONSYSTEM_H
#define ANIMATIONSYSTEM_H

#include "Coordinate.h"
#include <raylib-cpp.hpp>
#include <cmath>

class AnimationSystem {
public:
    static float lerp(float start, float end, float t) {
        return start + (end - start) * t;
    }
    
    static float easeInOut(float t) {
        return t < 0.5f ? 2 * t * t : 1 - std::pow(-2 * t + 2, 2) / 2;
    }
    
    static float easeOut(float t) {
        return 1 - std::pow(1 - t, 3);
    }
    
    static float easeIn(float t) {
        return t * t * t;
    }
    
    static float bounce(float t) {
        const float n1 = 7.5625f;
        const float d1 = 2.75f;
        
        if (t < 1 / d1) {
            return n1 * t * t;
        } else if (t < 2 / d1) {
            t -= 1.5f / d1;
            return n1 * t * t + 0.75f;
        } else if (t < 2.5f / d1) {
            t -= 2.25f / d1;
            return n1 * t * t + 0.9375f;
        } else {
            t -= 2.625f / d1;
            return n1 * t * t + 0.984375f;
        }
    }
    
    static Vector2 lerpPosition(Vector2 start, Vector2 end, float t) {
        return {lerp(start.x, end.x, t), lerp(start.y, end.y, t)};
    }
    
    static Color lerpColor(Color start, Color end, float t) {
        return {
            static_cast<unsigned char>(lerp(start.r, end.r, t)),
            static_cast<unsigned char>(lerp(start.g, end.g, t)),
            static_cast<unsigned char>(lerp(start.b, end.b, t)),
            static_cast<unsigned char>(lerp(start.a, end.a, t))
        };
    }
    
    static float pulse(float frequency = 5.0f) {
        return (std::sin(GetTime() * frequency) + 1.0f) * 0.5f;
    }
    
    static float wave(float frequency = 3.0f, float offset = 0.0f) {
        return std::sin(GetTime() * frequency + offset);
    }
};

struct SmoothPosition {
    Vector2 current;
    Vector2 target;
    float smoothSpeed;
    
    SmoothPosition(Vector2 initial = {0, 0}, float speed = 8.0f) 
        : current(initial), target(initial), smoothSpeed(speed) {}
    
    void setTarget(Vector2 newTarget) {
        target = newTarget;
    }
    
    void update() {
        float deltaTime = GetFrameTime();
        float t = 1.0f - std::exp(-smoothSpeed * deltaTime);
        current = AnimationSystem::lerpPosition(current, target, t);
    }
    
    bool isAtTarget(float threshold = 0.1f) const {
        float dx = target.x - current.x;
        float dy = target.y - current.y;
        return (dx * dx + dy * dy) < (threshold * threshold);
    }
};

#endif // ANIMATIONSYSTEM_H
