#include "UIManager.h"
#include "Coordinate.h"
#include <cmath>

UIManager::UIManager(int screenW, int screenH, int cellSz) 
    : screenWidth(screenW), screenHeight(screenH), cellSize(cellSz),
      hudHeight(Coordinate::HUD_ROWS * cellSz) {
}

void UIManager::drawHUD(int level, int score, int targetScore, int lives, 
                       float levelTimer, const std::vector<PowerUpEffect>& activePowerUps,
                       bool canFireHarpoon, float harpoonProgress) {
    DrawRectangle(0, 0, screenWidth, 2 * cellSize, Color{40, 40, 40, 255});
    DrawLine(0, 2 * cellSize, screenWidth, 2 * cellSize, WHITE);
    
    DrawText("UNDERGROUND ADVENTURE", 10, 10, 24, WHITE);
    DrawText(("Level " + std::to_string(level)).c_str(), 
             screenWidth - 120, 10, 20, BLUE);
    
    int row2Y = cellSize + 10;
    DrawText(("Score: " + std::to_string(score)).c_str(), 10, row2Y, 18, GREEN);
    DrawText(("Target: " + std::to_string(targetScore)).c_str(), 
             180, row2Y, 18, YELLOW);
    DrawText(("Lives: " + std::to_string(lives)).c_str(), 350, row2Y, 18, RED);
    
    int minutes = static_cast<int>(levelTimer) / 60;
    int seconds = static_cast<int>(levelTimer) % 60;
    DrawText(("Time: " + std::to_string(minutes) + ":" + 
             (seconds < 10 ? "0" : "") + std::to_string(seconds)).c_str(), 
             480, row2Y, 18, WHITE);
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
    
    Color textColor = ColorAlpha(YELLOW, alpha);
    DrawText(message.c_str(), x, y, 24, textColor);
}

void UIManager::drawMenu() {
    DrawText("UNDERGROUND ADVENTURE", screenWidth/2 - 200, 
             screenHeight/2 - 100, 32, WHITE);
    DrawText("Press ENTER to Start", screenWidth/2 - 120, 
             screenHeight/2 - 50, 20, YELLOW);
}

void UIManager::drawPauseOverlay() {
    DrawRectangle(0, hudHeight, screenWidth, screenHeight - hudHeight, 
                 ColorAlpha(BLACK, 0.7f));
    DrawText("PAUSED", screenWidth/2 - 60, screenHeight/2, 32, WHITE);
}

void UIManager::drawGameOverScreen(int score, int level) {
    DrawRectangle(0, hudHeight, screenWidth, screenHeight - hudHeight, 
                 ColorAlpha(BLACK, 0.8f));
    DrawText("GAME OVER", screenWidth/2 - 100, screenHeight/2 - 50, 32, RED);
    DrawText(("Final Score: " + std::to_string(score)).c_str(), 
             screenWidth/2 - 80, screenHeight/2, 20, WHITE);
}

void UIManager::drawLevelCompleteScreen(int score, float levelTimer) {
    DrawRectangle(0, hudHeight, screenWidth, screenHeight - hudHeight, 
                 ColorAlpha(BLACK, 0.7f));
    DrawText("LEVEL COMPLETE!", screenWidth/2 - 130, 
             screenHeight/2 - 50, 28, GREEN);
    DrawText(("Score: " + std::to_string(score)).c_str(), 
             screenWidth/2 - 60, screenHeight/2, 20, WHITE);
}

void UIManager::drawVictoryScreen(int score) {
    DrawRectangle(0, 0, screenWidth, screenHeight, ColorAlpha(PURPLE, 0.8f));
    DrawText("VICTORY!", screenWidth/2 - 80, screenHeight/2 - 80, 40, GOLD);
    DrawText(("Final Score: " + std::to_string(score)).c_str(), 
             screenWidth/2 - 80, screenHeight/2 + 10, 18, YELLOW);
}

void UIManager::drawPowerUpStatus(const std::vector<PowerUpEffect>& effects, 
                                 int startX, int y) {
    // Implementation for power-up status display
}
