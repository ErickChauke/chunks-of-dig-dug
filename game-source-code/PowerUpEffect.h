#ifndef POWERUPEFFECT_H
#define POWERUPEFFECT_H

#include "PowerUp.h"
#include <raylib-cpp.hpp>

/**
 * @file PowerUpEffect.h
 * @brief Tracks active timed power-up effects
 */

/**
 * @struct PowerUpEffect
 * @brief Represents an active timed power-up effect
 * 
 * PowerUpEffect tracks the lifecycle of timed power-up effects:
 * - Records when effect started
 * - Tracks total duration
 * - Calculates time remaining
 * - Determines when effect expires
 * 
 * Used by PowerUpManager to manage multiple simultaneous effects:
 * - RAPID_FIRE: 15 seconds
 * - POWER_SHOT: 20 seconds
 * - SPEED_BOOST: 12 seconds
 * - INVINCIBILITY: 10 seconds
 * 
 * Lifecycle:
 * 1. Created when power-up collected
 * 2. Added to PowerUpManager's active effects vector
 * 3. Checked each frame for expiration
 * 4. Removed when isExpired() returns true
 * 
 * Design benefits:
 * - Simple expiration checking (no manual timer tracking)
 * - Supports multiple simultaneous effects
 * - Automatic cleanup when expired
 * - Provides time remaining for UI display
 * 
 * @note Uses GetTime() for timestamp-based expiration
 */
struct PowerUpEffect {
    PowerUpType type;  ///< Type of power-up effect
    float startTime;   ///< Timestamp when effect activated
    float duration;    ///< Total effect duration (seconds)
    bool active;       ///< Is effect currently active
    
    /**
     * @brief Construct power-up effect
     * @param t Power-up type
     * @param d Effect duration in seconds
     */
    PowerUpEffect(PowerUpType t, float d) : type(t), startTime(GetTime()), 
                                           duration(d), active(true) {}
    
    /**
     * @brief Check if effect has expired
     * @return true if current time exceeds start + duration
     */
    bool isExpired() const {
        return (GetTime() - startTime) >= duration;
    }
    
    /**
     * @brief Get remaining effect time
     * @return float Seconds remaining (0.0 if expired)
     */
    float getTimeRemaining() const {
        return duration - (GetTime() - startTime);
    }
};

#endif // POWERUPEFFECT_H
