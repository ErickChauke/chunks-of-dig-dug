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
                DrawRectangleLines(screenX, screenY, cellSize, cellSize, DARKBROWN);
            } else {
                DrawRectangle(screenX, screenY, cellSize, cellSize, DARKGRAY);
                DrawRectangleLines(screenX, screenY, cellSize, cellSize, GRAY);
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
}

void RenderManager::drawPlayer(const Player& player, bool hasSpeedBoost, 
                              bool isDigging) {
    Coordinate pos = player.getPosition();
    int screenX = pos.col * cellSize + cellSize / 2;
    int screenY = pos.row * cellSize + cellSize / 2;
    
    DrawCircle(screenX, screenY, 16, YELLOW);
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
        
        Color color = (enemy.getEnemyType() == EnemyType::AGGRESSIVE_MONSTER) ? 
                     Color{139, 0, 0, 255} : RED;
        
        if (enemy.getIsPhasing()) {
            color = ColorAlpha(color, 0.7f);
        }
        
        DrawCircle(screenX, screenY, 15, color);
        DrawCircleLines(screenX, screenY, 15, MAROON);
    }
}

void RenderManager::drawHarpoons(const std::vector<Harpoon>& harpoons, 
                                bool hasPowerShot) {
    for (const auto& harpoon : harpoons) {
        if (!harpoon.isActive()) continue;
        
        const auto& segments = harpoon.getSegments();
        Color color = hasPowerShot ? ORANGE : GREEN;
        
        for (const auto& segment : segments) {
            int screenX = segment.col * cellSize + cellSize / 2;
            int screenY = segment.row * cellSize + cellSize / 2;
            DrawCircle(screenX, screenY, 4, color);
        }
    }
}

void RenderManager::drawPowerUps(const std::vector<PowerUp>& powerUps) {
    for (const auto& powerUp : powerUps) {
        if (!powerUp.isActive()) continue;
        
        Coordinate pos = powerUp.getPosition();
        int screenX = pos.col * cellSize + cellSize / 2;
        int screenY = pos.row * cellSize + cellSize / 2;
        
        float pulse = sin(GetTime() * 5.0f) * 0.3f + 0.7f;
        Color color = ColorAlpha(GOLD, pulse);
        
        DrawCircle(screenX, screenY, 12, color);
        DrawText("P", screenX - 5, screenY - 8, 16, BLACK);
    }
}

void RenderManager::drawRocks(const std::vector<Rock>& rocks, const Player& player) {
    for (const auto& rock : rocks) {
        if (!rock.isActive()) continue;
        
        Coordinate pos = rock.getPosition();
        int screenX = pos.col * cellSize + cellSize / 2;
        int screenY = pos.row * cellSize + cellSize / 2;
        
        Color rockColor = rock.getIsFalling() ? RED : PURPLE;
        
        DrawCircle(screenX, screenY, 18, rockColor);
        DrawCircleLines(screenX, screenY, 18, WHITE);
        DrawText("ROCK", screenX - 15, screenY - 6, 12, WHITE);
    }
}
