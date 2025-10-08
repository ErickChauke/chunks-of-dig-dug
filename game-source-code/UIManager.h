#ifndef UIMANAGER_H
#define UIMANAGER_H

#include <raylib-cpp.hpp>
#include <vector>
#include <string>
#include "PowerUp.h"

/**
 * @file UIManager.h
 * @brief User interface rendering for HUD, menus, and overlays
 */

/**
 * @struct PowerUpEffect
 * @brief Tracks active timed power-up effect
 */
struct PowerUpEffect {
    PowerUpType type;
    float startTime;
    float duration;
    bool active;
    
    PowerUpEffect(PowerUpType t, float d) : type(t), startTime(GetTime()), 
                                           duration(d), active(true) {}
    
    bool isExpired() const {
        return (GetTime() - startTime) >= duration;
    }
    
    float getTimeRemaining() const {
        return duration - (GetTime() - startTime);
    }
};

/**
 * @class UIManager
 * @brief Manages all user interface rendering
 * 
 * UIManager handles non-gameplay visual elements:
 * - HUD (score, lives, level, timer, power-ups)
 * - Menus (main menu, pause overlay)
 * - Game state screens (game over, level complete, victory)
 * - Notifications (power-up collection messages)
 * - Status indicators (weapon cooldown bars)
 * 
 * HUD layout (top 2 rows):
 * Row 1: Title, Level indicator
 * Row 2: Score, Target, Lives, Timer, Active power-ups, Cooldown
 * 
 * Visual feedback:
 * - Pulsing text for low lives
 * - Fading notifications
 * - Cooldown progress bars
 * - Color-coded power-up timers
 * 
 * Screen overlays:
 * - Semi-transparent backgrounds
 * - Centered text layouts
 * - Instruction prompts
 * 
 * @note UIManager only renders - no game logic
 */
class UIManager {
private:
    const int screenWidth;
    const int screenHeight;
    const int cellSize;
    const int hudHeight;

public:
    /**
     * @brief Construct UI manager with screen dimensions
     * @param screenW Screen width in pixels
     * @param screenH Screen height in pixels
     * @param cellSz Cell size in pixels
     */
    UIManager(int screenW, int screenH, int cellSz);
    
    /**
     * @brief Draw heads-up display during gameplay
     * @param level Current level number
     * @param score Current score
     * @param targetScore Target score for completion
     * @param lives Remaining lives
     * @param levelTimer Time elapsed in level
     * @param activePowerUps Active timed effects
     * @param canFireHarpoon true if weapon ready
     * @param harpoonProgress Cooldown progress (0.0-1.0)
     */
    void drawHUD(int level, int score, int targetScore, int lives, 
                float levelTimer, const std::vector<PowerUpEffect>& activePowerUps,
                bool canFireHarpoon, float harpoonProgress);
    
    /**
     * @brief Draw power-up collection notification
     * @param message Notification text
     * @param timeSince Time since collection
     */
    void drawPowerUpNotification(const std::string& message, float timeSince);
    
    /**
     * @brief Draw main menu screen
     */
    void drawMenu();
    
    /**
     * @brief Draw pause overlay
     */
    void drawPauseOverlay();
    
    /**
     * @brief Draw game over screen
     * @param score Final score
     * @param level Level reached
     */
    void drawGameOverScreen(int score, int level);
    
    /**
     * @brief Draw level complete screen
     * @param score Current score
     * @param levelTimer Time taken
     */
    void drawLevelCompleteScreen(int score, float levelTimer);
    
    /**
     * @brief Draw victory screen
     * @param score Final score
     */
    void drawVictoryScreen(int score);
    
private:
    void drawPowerUpStatus(const std::vector<PowerUpEffect>& effects, 
                          int startX, int y);
    void drawWeaponCooldown(bool canFire, float progress, int x, int y);
};

#endif // UIMANAGER_H
