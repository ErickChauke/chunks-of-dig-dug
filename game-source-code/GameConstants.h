#ifndef GAMECONSTANTS_H
#define GAMECONSTANTS_H

namespace GameConstants {
    // Screen and rendering
    const int SCREEN_WIDTH = 1200;
    const int SCREEN_HEIGHT = 800;
    const int CELL_SIZE = 40;
    
    // Player mechanics
    const float BASE_MOVE_COOLDOWN = 0.12f;
    const float FAST_MOVE_COOLDOWN = 0.08f;
    const float DIG_EFFECT_DURATION = 0.3f;
    const int CONSECUTIVE_MOVES_FOR_SPEEDUP = 4;
    
    // Weapon system
    const float HARPOON_COOLDOWN_TIME = 0.8f;
    const float HARPOON_SPEED = 5.0f;
    const float HARPOON_MAX_RANGE = 4.0f;
    const float RAPID_FIRE_COOLDOWN = 0.25f;
    
    // Enemy behavior
    const float ENEMY_STUN_DURATION = 0.5f;
    const float ENEMY_MOVE_CHECK_INTERVAL = 0.1f;
    
    // Rock physics
    const float ROCK_FALL_SPEED = 0.18f;
    const float ROCK_CRUSH_DELAY = 1.2f;
    const float ROCK_STATIONARY_CRUSH_DELAY = 0.4f;
    const float ROCK_STABILITY_CHECK_INTERVAL = 0.1f;
    
    // Power-ups
    const float POWERUP_LIFETIME = 25.0f;
    const float POWERUP_SPAWN_INTERVAL = 18.0f;
    const float POWERUP_NOTIFICATION_DURATION = 2.5f;
    
    // Level progression
    const int STARTING_LIVES = 3;
    const int BASE_TARGET_SCORE = 1000;
    const float MAX_LEVEL_TIME = 180.0f;
    const int MAX_LEVELS = 10;
}

#endif // GAMECONSTANTS_H
