#include "RenderManager.h"
#include <cmath>

RenderManager::RenderManager(int cellSz, int screenW, int screenH) 
    : cellSize(cellSz), screenWidth(screenW), screenHeight(screenH),
      hudHeight(Coordinate::HUD_ROWS * cellSz) {
}

void RenderManager::drawTerrain(const BlockGrid& terrain) {
    for (int row = 0; row < Coordinate::WORLD_ROWS; ++row) {
        for (int col = 0; col < Coordinate::WORLD_COLS; ++col) {
            Coordinate pos(row, col);
            int screenX = col * cellSize;
            int screenY = row * cellSize;
            
            if (row < Coordinate::HUD_ROWS) {
                if (row == 2) {
                    drawSkyCell(screenX, screenY);
                }
            } else if (terrain.isLocationBlocked(pos)) {
                DrawRectangle(screenX, screenY, cellSize, cellSize, BROWN);
                DrawRectangleLines(screenX, screenY, cellSize, cellSize, 
                                 Color{101, 67, 33, 255});
                DrawCircle(screenX + cellSize/4, screenY + cellSize/4, 2, 
                         Color{101, 67, 33, 255});
            } else {
                DrawRectangle(screenX, screenY, cellSize, cellSize, DARKGRAY);
                DrawRectangleLines(screenX, screenY, cellSize, cellSize, GRAY);
                DrawCircle(screenX + cellSize/2, screenY + cellSize/2, 1, LIGHTGRAY);
            }
        }
    }
}

void RenderManager::drawSkyCell(int x, int y) {
    float time = GetTime() * 0.3f;
    float skyVariation = sin(time + x * 0.01f) * 0.1f + 0.9f;
    Color skyColor = Color{
        static_cast<unsigned char>(135 * skyVariation),
        static_cast<unsigned char>(206 * skyVariation),
        static_cast<unsigned char>(235 * skyVariation),
        255
    };
    
    DrawRectangle(x, y, cellSize, cellSize, skyColor);
    
    float cloudX = fmod(x + time * 20, screenWidth + 40);
    float cloudY = y + sin(time + x * 0.05f) * 3;
    
    if (cloudX > x - 20 && cloudX < x + cellSize + 20) {
        Color cloudColor = Color{255, 255, 255, 120};
        DrawCircle(static_cast<int>(cloudX), static_cast<int>(cloudY + cellSize/2), 
                  6, cloudColor);
        DrawCircle(static_cast<int>(cloudX + 8), 
                  static_cast<int>(cloudY + cellSize/2), 4, cloudColor);
    }
}

void RenderManager::drawPlayer(const Player& player, bool hasSpeedBoost, 
                              bool isDigging) {
    Coordinate pos = player.getPosition();
    int screenX = pos.col * cellSize + cellSize / 2;
    int screenY = pos.row * cellSize + cellSize / 2;
    
    Color playerColor = YELLOW;
    
    if (hasSpeedBoost) {
        float pulse = sin(GetTime() * 8.0f) * 0.4f + 0.6f;
        DrawCircleLines(screenX, screenY, 20, ColorAlpha(YELLOW, pulse));
    }
    
    DrawCircle(screenX, screenY, 16, playerColor);
    DrawCircleLines(screenX, screenY, 16, ORANGE);
    DrawCircle(screenX - 6, screenY - 6, 3, BLACK);
    DrawCircle(screenX + 6, screenY - 6, 3, BLACK);
    
    if (isDigging) {
        float pulse = sin(GetTime() * 10.0f) * 0.4f + 0.6f;
        DrawCircleLines(screenX, screenY, 22, ColorAlpha(ORANGE, pulse));
    }
}

void RenderManager::drawEnemies(const std::vector<Enemy>& enemies) {
    for (const auto& enemy : enemies) {
        if (!enemy.isActive()) continue;
        
        Coordinate pos = enemy.getPosition();
        int screenX = pos.col * cellSize + cellSize / 2;
        int screenY = pos.row * cellSize + cellSize / 2;
        
        if (enemy.getIsDestroyed()) {
            float progress = enemy.getDestroyProgress();
            float scale = 1.0f + progress * 2.0f;
            float alpha = 1.0f - progress;
            
            Color destroyColor = Color{255, 255, 0, 
                                     static_cast<unsigned char>(255 * alpha)};
            
            for (int i = 0; i < 3; ++i) {
                float circleScale = scale + i * 0.3f;
                int radius = static_cast<int>(15 * circleScale);
                DrawCircleLines(screenX, screenY, radius, destroyColor);
            }
            
            for (int i = 0; i < 8; ++i) {
                float angle = (i * 45.0f) * DEG2RAD;
                float distance = progress * 30;
                int px = screenX + static_cast<int>(cos(angle) * distance);
                int py = screenY + static_cast<int>(sin(angle) * distance);
                DrawCircle(px, py, 2, destroyColor);
            }
        } else {
            Color color = (enemy.getEnemyType() == EnemyType::AGGRESSIVE_MONSTER) ? 
                         Color{139, 0, 0, 255} : RED;
            
            if (enemy.getIsPhasing()) {
                color = ColorAlpha(color, 0.7f);
            }
            
            DrawCircle(screenX, screenY, 15, color);
            DrawCircleLines(screenX, screenY, 15, MAROON);
            DrawCircle(screenX - 5, screenY - 5, 2, BLACK);
            DrawCircle(screenX + 5, screenY - 5, 2, BLACK);
        }
    }
}

void RenderManager::drawHarpoons(const std::vector<Harpoon>& harpoons, 
                                bool hasPowerShot) {
    for (const auto& harpoon : harpoons) {
        if (!harpoon.isActive()) continue;
        
        const auto& segments = harpoon.getSegments();
        Color color = hasPowerShot ? Color{255, 100, 0, 255} : 
                                   Color{50, 255, 50, 255};
        
        for (size_t i = 0; i < segments.size(); ++i) {
            Coordinate pos = segments[i];
            int screenX = pos.col * cellSize + cellSize / 2;
            int screenY = pos.row * cellSize + cellSize / 2;
            
            if (i == 0) {
                DrawCircle(screenX, screenY, 4, color);
                DrawCircleLines(screenX, screenY, 5, WHITE);
            } else if (i == segments.size() - 1) {
                int tipSize = hasPowerShot ? 7 : 5;
                DrawCircle(screenX, screenY, tipSize, color);
                DrawCircleLines(screenX, screenY, tipSize + 2, WHITE);
                DrawCircleLines(screenX, screenY, tipSize + 4, 
                              ColorAlpha(color, 0.5f));
            } else {
                DrawCircle(screenX, screenY, 3, color);
            }
            
            if (i > 0) {
                Coordinate prevPos = segments[i-1];
                int prevX = prevPos.col * cellSize + cellSize / 2;
                int prevY = prevPos.row * cellSize + cellSize / 2;
                DrawLine(prevX, prevY, screenX, screenY, color);
                DrawLine(prevX, prevY, screenX, screenY, ColorAlpha(WHITE, 0.3f));
            }
        }
    }
}

void RenderManager::drawPowerUps(const std::vector<PowerUp>& powerUps) {
    for (const auto& powerUp : powerUps) {
        if (!powerUp.isActive()) continue;
        
        Coordinate pos = powerUp.getPosition();
        int screenX = pos.col * cellSize + cellSize / 2;
        int screenY = pos.row * cellSize + cellSize / 2;
        
        Color color;
        std::string symbol;
        
        switch (powerUp.getType()) {
            case PowerUpType::EXTRA_LIFE: 
                color = GREEN; symbol = "+"; break;
            case PowerUpType::SCORE_MULTIPLIER: 
                color = GOLD; symbol = "$"; break;
            case PowerUpType::RAPID_FIRE: 
                color = Color{0, 255, 255, 255}; symbol = "R"; break;
            case PowerUpType::POWER_SHOT: 
                color = ORANGE; symbol = "P"; break;
            case PowerUpType::SPEED_BOOST:
                color = YELLOW; symbol = "S"; break;
            default: 
                color = WHITE; symbol = "?"; break;
        }
        
        float pulse = sin(GetTime() * 5.0f) * 0.3f + 0.7f;
        color = ColorAlpha(color, pulse);
        
        DrawCircle(screenX, screenY, 12, color);
        DrawCircleLines(screenX, screenY, 14, WHITE);
        
        int symbolSize = 16;
        int symbolWidth = MeasureText(symbol.c_str(), symbolSize);
        DrawText(symbol.c_str(), screenX - symbolWidth/2, 
                screenY - symbolSize/2, symbolSize, BLACK);
    }
}

void RenderManager::drawRocks(const std::vector<Rock>& rocks, const Player& player) {
    for (const auto& rock : rocks) {
        if (!rock.isActive()) continue;
        
        Coordinate pos = rock.getPosition();
        int screenX = pos.col * cellSize + cellSize / 2;
        int screenY = pos.row * cellSize + cellSize / 2;
        
        Color rockColor = PURPLE;
        
        if (rock.getIsFalling()) {
            float pulse = sin(GetTime() * 10.0f) * 0.3f + 0.7f;
            rockColor = ColorAlpha(RED, pulse);
            
            DrawCircle(screenX, screenY - 5, 16, ColorAlpha(RED, 0.3f));
            DrawCircle(screenX, screenY - 10, 14, ColorAlpha(RED, 0.2f));
            
            // Check if player is in danger zone for visual warning
            // Note: We can't call checkPlayerCrush here because it's not const
            // Instead, we'll do a simple visual check for warning display
            Coordinate playerPos = player.getPosition();
            if (pos.col == playerPos.col && pos.row < playerPos.row) {
                // Player is under falling rock - show warning
                std::string warningText = "DANGER!";
                
                DrawRectangle(screenX - 30, screenY - 40, 60, 20, 
                            ColorAlpha(RED, 0.8f));
                DrawText(warningText.c_str(), screenX - 25, screenY - 35, 12, WHITE);
            }
        }
        
        DrawCircle(screenX, screenY, 18, rockColor);
        DrawCircleLines(screenX, screenY, 18, WHITE);
        DrawText("ROCK", screenX - 15, screenY - 6, 12, WHITE);
        
        if (rock.getIsFalling()) {
            DrawCircle(screenX + 2, screenY + 2, 18, ColorAlpha(BLACK, 0.5f));
        }
    }
}
