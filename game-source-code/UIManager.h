#ifndef UIMANAGER_H
#define UIMANAGER_H

#include <raylib-cpp.hpp>
#include <vector>
#include <string>
#include "PowerUp.h"

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

class UIManager {
private:
    const int screenWidth;
    const int screenHeight;
    const int cellSize;
    const int hudHeight;

public:
    UIManager(int screenW, int screenH, int cellSz);
    
    void drawHUD(int level, int score, int targetScore, int lives, 
                float levelTimer, const std::vector<PowerUpEffect>& activePowerUps,
                bool canFireHarpoon, float harpoonProgress);
    
    void drawPowerUpNotification(const std::string& message, float timeSince);
    void drawMenu();
    void drawPauseOverlay();
    void drawGameOverScreen(int score, int level);
    void drawLevelCompleteScreen(int score, float levelTimer);
    void drawVictoryScreen(int score);
    
private:
    void drawPowerUpStatus(const std::vector<PowerUpEffect>& effects, 
                          int startX, int y);
    void drawWeaponCooldown(bool canFire, float progress, int x, int y);
};

#endif // UIMANAGER_H
