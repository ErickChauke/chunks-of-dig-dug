#include "UIManager.h"
#include "Coordinate.h"
#include <cmath>
#include <algorithm>

UIManager::UIManager(int screenW, int screenH, int cellSz) 
    : screenWidth(screenW), screenHeight(screenH), cellSize(cellSz),
      hudHeight(Coordinate::HUD_ROWS * cellSz) {
}

void UIManager::drawHUD(int level, int score, int targetScore, int lives, 
                       float levelTimer, const std::vector<PowerUpEffect>& activePowerUps,
                       bool canFireHarpoon, float harpoonProgress) {
    DrawRectangle(0, 0, screenWidth, 2 * cellSize, Color{40, 40, 40, 255});
    DrawLine(0, 2 * cellSize, screenWidth, 2 * cellSize, WHITE);
    
    DrawText("UNDERGROUND ADVENTURE", 10, 10, 24, GOLD);
    DrawText(("Level " + std::to_string(level)).c_str(), 
             screenWidth - 120, 10, 20, SKYBLUE);
    
    int row2Y = cellSize + 10;
    DrawText(("Score: " + std::to_string(score)).c_str(), 10, row2Y, 18, GREEN);
    DrawText(("Target: " + std::to_string(targetScore)).c_str(), 
             180, row2Y, 18, YELLOW);
    
    Color livesColor = lives > 1 ? Color{255, 100, 100, 255} : RED;
    DrawText(("Lives: " + std::to_string(lives)).c_str(), 350, row2Y, 18, livesColor);
    
    int minutes = static_cast<int>(levelTimer) / 60;
    int seconds = static_cast<int>(levelTimer) % 60;
    DrawText(("Time: " + std::to_string(minutes) + ":" + 
             (seconds < 10 ? "0" : "") + std::to_string(seconds)).c_str(), 
             480, row2Y, 18, WHITE);
    
    drawPowerUpStatus(activePowerUps, 620, row2Y);
    
    if (!canFireHarpoon) {
        drawWeaponCooldown(canFireHarpoon, harpoonProgress, 
                          screenWidth - 100, row2Y + 25);
    }
}

void UIManager::drawPowerUpStatus(const std::vector<PowerUpEffect>& effects, 
                                 int startX, int y) {
    int powerupX = startX;
    
    for (const auto& effect : effects) {
        if (!effect.active) continue;
        
        std::string text;
        Color color;
        float timeLeft = effect.getTimeRemaining();
        
        switch (effect.type) {
            case PowerUpType::RAPID_FIRE:
                text = "RAPID " + std::to_string(static_cast<int>(timeLeft)) + "s";
                color = Color{0, 255, 255, 255};
                break;
            case PowerUpType::POWER_SHOT:
                text = "POWER " + std::to_string(static_cast<int>(timeLeft)) + "s";
                color = ORANGE;
                break;
            case PowerUpType::SPEED_BOOST:
                text = "SPEED " + std::to_string(static_cast<int>(timeLeft)) + "s";
                color = Color{255, 255, 0, 255};
                break;
            case PowerUpType::INVINCIBILITY:
                text = "INVINCIBLE " + std::to_string(static_cast<int>(timeLeft)) + "s";
                color = Color{255, 0, 255, 255};
                break;
            default:
                continue;
        }
        
        if (timeLeft < 3.0f) {
            float pulse = sin(GetTime() * 8.0f) * 0.3f + 0.7f;
            color = ColorAlpha(color, pulse);
        }
        
        DrawText(text.c_str(), powerupX, y, 14, color);
        powerupX += text.length() * 8 + 10;
    }
}

void UIManager::drawWeaponCooldown(bool canFire, float progress, int x, int y) {
    int barWidth = 80;
    int barHeight = 8;
    
    DrawRectangle(x, y, barWidth, barHeight, GRAY);
    DrawRectangle(x, y, static_cast<int>(barWidth * progress), barHeight, GREEN);
    DrawText("WEAPON", x - 60, y, 12, WHITE);
}

void UIManager::drawPowerUpNotification(const std::string& message, 
                                       float timeSince) {
    if (timeSince >= 3.0f || message.empty()) return;
    
    float alpha = 1.0f - (timeSince / 3.0f);
    int textWidth = MeasureText(message.c_str(), 24);
    int x = (screenWidth - textWidth) / 2;
    int y = screenHeight / 2 - 100;
    
    DrawRectangle(x - 10, y - 5, textWidth + 20, 35, 
                 ColorAlpha(BLACK, alpha * 0.7f));
    
    Color textColor = ColorAlpha(GOLD, alpha);
    DrawText(message.c_str(), x + 1, y + 1, 24, ColorAlpha(BLACK, alpha));
    DrawText(message.c_str(), x, y, 24, textColor);
}

void UIManager::drawMenu() {
    DrawText("UNDERGROUND ADVENTURE", screenWidth/2 - 200, 
             screenHeight/2 - 100, 32, GOLD);
    DrawText("Press ENTER to Start", screenWidth/2 - 120, 
             screenHeight/2 - 40, 20, YELLOW);
    DrawText("Arrow Keys - Move & Dig", screenWidth/2 - 120, 
             screenHeight/2, 16, LIGHTGRAY);
    DrawText("SPACE - Fire Harpoon", screenWidth/2 - 110, 
             screenHeight/2 + 25, 16, LIGHTGRAY);
    DrawText("P - Pause  |  R - Restart", screenWidth/2 - 120, 
             screenHeight/2 + 50, 14, GRAY);
}

void UIManager::drawPauseOverlay() {
    DrawRectangle(0, hudHeight, screenWidth, screenHeight - hudHeight, 
                 ColorAlpha(BLACK, 0.7f));
    DrawText("PAUSED", screenWidth/2 - 60, screenHeight/2, 32, WHITE);
    DrawText("Press P to Resume", screenWidth/2 - 80, 
             screenHeight/2 + 40, 16, YELLOW);
}

void UIManager::drawGameOverScreen(int score, int level) {
    DrawRectangle(0, hudHeight, screenWidth, screenHeight - hudHeight, 
                 ColorAlpha(BLACK, 0.8f));
    DrawText("GAME OVER", screenWidth/2 - 100, screenHeight/2 - 50, 32, RED);
    DrawText(("Final Score: " + std::to_string(score)).c_str(), 
             screenWidth/2 - 80, screenHeight/2, 20, WHITE);
    DrawText(("Level Reached: " + std::to_string(level)).c_str(), 
             screenWidth/2 - 90, screenHeight/2 + 30, 16, GRAY);
    DrawText("Press R to Restart", screenWidth/2 - 80, 
             screenHeight/2 + 60, 16, YELLOW);
    DrawText("Press ESC to Exit", screenWidth/2 - 75, 
             screenHeight/2 + 80, 16, YELLOW);
}

void UIManager::drawLevelCompleteScreen(int score, float levelTimer) {
    DrawRectangle(0, hudHeight, screenWidth, screenHeight - hudHeight, 
                 ColorAlpha(BLACK, 0.7f));
    DrawText("LEVEL COMPLETE!", screenWidth/2 - 130, 
             screenHeight/2 - 50, 28, GREEN);
    DrawText(("Score: " + std::to_string(score)).c_str(), 
             screenWidth/2 - 60, screenHeight/2, 20, WHITE);
    
    float maxTime = 180.0f;
    float timeRatio = std::max(0.0f, (maxTime - levelTimer) / maxTime);
    int timeBonus = static_cast<int>(1000 * timeRatio);
    DrawText(("Time Bonus: " + std::to_string(timeBonus)).c_str(), 
             screenWidth/2 - 80, screenHeight/2 + 30, 16, GOLD);
    
    DrawText("Press ENTER for Next Level", screenWidth/2 - 120, 
             screenHeight/2 + 60, 16, YELLOW);
}

void UIManager::drawVictoryScreen(int score) {
    DrawRectangle(0, 0, screenWidth, screenHeight, ColorAlpha(PURPLE, 0.8f));
    DrawText("VICTORY!", screenWidth/2 - 80, screenHeight/2 - 80, 40, GOLD);
    DrawText("You defeated all enemies!", screenWidth/2 - 140, 
             screenHeight/2 - 30, 20, WHITE);
    DrawText(("Final Score: " + std::to_string(score)).c_str(), 
             screenWidth/2 - 80, screenHeight/2 + 10, 18, YELLOW);
    DrawText("Press R to Play Again", screenWidth/2 - 90, 
             screenHeight/2 + 50, 16, WHITE);
}
