#ifndef SCREENSHAKE_H
#define SCREENSHAKE_H

#include <raylib-cpp.hpp>
#include <cmath>
#include <cstdlib>

/**
 * @file ScreenShake.h
 * @brief Camera shake effect for impact feedback
 */

/**
 * @class ScreenShake
 * @brief Creates screen shake effect for dramatic feedback
 * 
 * ScreenShake provides juicy camera shake effects that enhance
 * the feel of impacts and explosions:
 * - Random offset applied to entire screen
 * - Intensity decays over duration
 * - Easy to trigger with shake(intensity, duration)
 * 
 * Usage pattern:
 * 1. Call shake(intensity, duration) on impact event
 * 2. Call update() every frame to decay
 * 3. Call apply() before drawing game
 * 4. Call reset() after drawing to restore camera
 * 
 * Example intensities:
 * - Rock landing: shake(8.0f, 0.25s)
 * - Enemy destroyed: shake(5.0f, 0.2s)
 * - Player hit: shake(15.0f, 0.4s)
 * - Fire breath: shake(3.0f, 0.15s)
 * 
 * Technical implementation:
 * - Uses raylib's rlPushMatrix/rlPopMatrix for transforms
 * - Random offset recalculated each frame
 * - Linear decay from full intensity to zero
 * - Offset magnitude = intensity × (1 - progress)
 * 
 * Design benefits:
 * - Adds "game feel" and impact weight
 * - Simple API (one method to trigger)
 * - No permanent screen distortion
 * - Works with all rendering
 * 
 * Performance:
 * - Minimal overhead (simple math per frame)
 * - Only active during shake duration
 * - No allocations or complex calculations
 * 
 * @note Screen shake is pure visual feedback - no gameplay impact
 */
class ScreenShake {
private:
    float intensity;   ///< Maximum offset magnitude (pixels)
    float duration;    ///< Total shake duration (seconds)
    float elapsed;     ///< Time elapsed in current shake
    Vector2 offset;    ///< Current frame offset (x, y pixels)
    bool active;       ///< Is shake currently active

public:
    ScreenShake() : intensity(0.0f), duration(0.0f), elapsed(0.0f), 
                   offset({0.0f, 0.0f}), active(false) {}
    
    /**
     * @brief Trigger screen shake effect
     * @param intensityAmount Maximum offset (pixels, typically 3-15)
     * @param durationSeconds Total shake time (typically 0.15-0.4s)
     */
    void shake(float intensityAmount, float durationSeconds) {
        intensity = intensityAmount;
        duration = durationSeconds;
        elapsed = 0.0f;
        active = true;
    }
    
    /**
     * @brief Update shake progress and calculate offset
     * @note Call every frame
     */
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
    
    /**
     * @brief Get current frame offset
     * @return Vector2 Offset (x, y in pixels)
     */
    Vector2 getOffset() const {
        return offset;
    }
    
    /**
     * @brief Check if shake is active
     * @return true if currently shaking
     */
    bool isActive() const {
        return active;
    }
    
    /**
     * @brief Apply shake transform before drawing
     * @note Must call reset() after drawing complete
     */
    void apply() const {
        if (active) {
            rlPushMatrix();
            rlTranslatef(offset.x, offset.y, 0.0f);
        }
    }
    
    /**
     * @brief Reset transform after drawing
     * @note Must be called if apply() was called
     */
    void reset() const {
        if (active) {
            rlPopMatrix();
        }
    }
};

#endif // SCREENSHAKE_H
