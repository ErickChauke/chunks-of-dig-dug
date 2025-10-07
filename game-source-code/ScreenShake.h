#ifndef SCREENSHAKE_H
#define SCREENSHAKE_H

#include <raylib-cpp.hpp>
#include <cmath>
#include <cstdlib>

class ScreenShake {
private:
    float intensity;
    float duration;
    float elapsed;
    Vector2 offset;
    bool active;

public:
    ScreenShake() : intensity(0.0f), duration(0.0f), elapsed(0.0f), 
                   offset({0.0f, 0.0f}), active(false) {}
    
    void shake(float intensityAmount, float durationSeconds) {
        intensity = intensityAmount;
        duration = durationSeconds;
        elapsed = 0.0f;
        active = true;
    }
    
    void update() {
        if (!active) return;
        
        elapsed += GetFrameTime();
        
        if (elapsed >= duration) {
            active = false;
            offset = {0.0f, 0.0f};
            return;
        }
        
        float progress = elapsed / duration;
        float currentIntensity = intensity * (1.0f - progress);
        
        offset.x = (rand() % 200 - 100) * 0.01f * currentIntensity;
        offset.y = (rand() % 200 - 100) * 0.01f * currentIntensity;
    }
    
    Vector2 getOffset() const {
        return offset;
    }
    
    bool isActive() const {
        return active;
    }
    
    void apply() const {
        if (active) {
            rlPushMatrix();
            rlTranslatef(offset.x, offset.y, 0.0f);
        }
    }
    
    void reset() const {
        if (active) {
            rlPopMatrix();
        }
    }
};

#endif // SCREENSHAKE_H
