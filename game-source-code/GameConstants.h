#ifndef GAMECONSTANTS_H
#define GAMECONSTANTS_H

/**
 * @file GameConstants.h
 * @brief Global game configuration constants
 */

/**
 * @namespace GameConstants
 * @brief Centralized game balance and configuration values
 * 
 * GameConstants provides a single source of truth for all
 * game timing, physics, and balance parameters. Grouping
 * these values enables easy tweaking without searching
 * through multiple files.
 * 
 * Categories:
 * - Screen and rendering: Display dimensions, cell size
 * - Player mechanics: Movement speeds, cooldowns
 * - Weapon system: Harpoon timing and range
 * - Enemy behavior: AI timing intervals
 * - Rock physics: Gravity and crushing delays
 * - Power-ups: Spawn timing and durations
 * - Level progression: Lives, scoring, time limits
 * 
 * Design benefits:
 * - Easy game balance adjustments
 * - No magic numbers in code
 * - Clear documentation of values
 * - Single place to tune difficulty
 * 
 * @note All timing values are in seconds
 */
namespace GameConstants {
    // Screen and rendering
    const int SCREEN_WIDTH = 1200;   ///< Window width in pixels
    const int SCREEN_HEIGHT = 800;   ///< Window height in pixels
    const int CELL_SIZE = 40;        ///< Grid cell size (40×40 pixels)
    
    // Player mechanics
    const float BASE_MOVE_COOLDOWN = 0.12f;  ///< Base time between moves (seconds)
    const float FAST_MOVE_COOLDOWN = 0.08f;  ///< Speedup after consecutive moves
    const float DIG_EFFECT_DURATION = 0.3f;  ///< Dig animation duration (seconds)
    const int CONSECUTIVE_MOVES_FOR_SPEEDUP = 4; ///< Moves needed for speed boost
    
    // Weapon system
    const float HARPOON_COOLDOWN_TIME = 0.8f;  ///< Normal harpoon cooldown (seconds)
    const float HARPOON_SPEED = 5.0f;          ///< Harpoon extension speed (cells/sec)
    const float HARPOON_MAX_RANGE = 4.0f;      ///< Maximum harpoon reach (cells)
    const float RAPID_FIRE_COOLDOWN = 0.25f;   ///< Rapid fire cooldown (seconds)
    
    // Enemy behavior
    const float ENEMY_STUN_DURATION = 0.5f;       ///< Stun time after damage (seconds)
    const float ENEMY_MOVE_CHECK_INTERVAL = 0.1f; ///< AI decision frequency (seconds)
    
    // Rock physics
    const float ROCK_FALL_SPEED = 0.18f;            ///< Time per cell fall (seconds)
    const float ROCK_CRUSH_DELAY = 1.2f;            ///< Crush moving player (seconds)
    const float ROCK_STATIONARY_CRUSH_DELAY = 0.4f; ///< Crush stationary player (seconds)
    const float ROCK_STABILITY_CHECK_INTERVAL = 0.1f; ///< Support check frequency (seconds)
    
    // Power-ups
    const float POWERUP_LIFETIME = 25.0f;          ///< Despawn time (seconds)
    const float POWERUP_SPAWN_INTERVAL = 18.0f;   ///< Average spawn interval (seconds)
    const float POWERUP_NOTIFICATION_DURATION = 2.5f; ///< UI message duration (seconds)
    
    // Level progression
    const int STARTING_LIVES = 3;        ///< Lives at game start
    const int BASE_TARGET_SCORE = 1000;  ///< Initial target score
    const float MAX_LEVEL_TIME = 180.0f; ///< Maximum level time (3 minutes)
    const int MAX_LEVELS = 10;           ///< Total levels in game
}

#endif // GAMECONSTANTS_H
