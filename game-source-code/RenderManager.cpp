#include "RenderManager.h"
#include "AnimationSystem.h"
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
                drawEarthBlock(screenX, screenY);
            } else {
                drawTunnelCell(screenX, screenY);
            }
        }
    }
}

void RenderManager::drawSkyCell(int x, int y) {
    float time = GetTime() * 0.3f;
    float skyVariation = AnimationSystem::wave(0.5f, x * 0.01f) * 0.1f + 0.9f;
    Color skyColor = Color{
        static_cast<unsigned char>(135 * skyVariation),
        static_cast<unsigned char>(206 * skyVariation),
        static_cast<unsigned char>(235 * skyVariation),
        255
    };
    
    DrawRectangle(x, y, cellSize, cellSize, skyColor);
    
    float cloudX = fmod(x + time * 20, screenWidth + 40);
    float cloudY = y + AnimationSystem::wave(3.0f, x * 0.05f) * 3;
    
    if (cloudX > x - 20 && cloudX < x + cellSize + 20) {
        Color cloudColor = Color{255, 255, 255, 120};
        DrawCircle(static_cast<int>(cloudX), static_cast<int>(cloudY + cellSize/2), 
                  6, cloudColor);
        DrawCircle(static_cast<int>(cloudX + 8), 
                  static_cast<int>(cloudY + cellSize/2), 4, cloudColor);
    }
}

void RenderManager::drawEarthBlock(int x, int y) {
    DrawRectangle(x, y, cellSize, cellSize, BROWN);
    DrawRectangleLines(x, y, cellSize, cellSize, Color{101, 67, 33, 255});
    
    float offset = AnimationSystem::wave(2.0f, x * 0.1f + y * 0.1f) * 1.5f;
    DrawCircle(x + cellSize/4 + static_cast<int>(offset), 
              y + cellSize/4, 2, Color{101, 67, 33, 255});
}

void RenderManager::drawTunnelCell(int x, int y) {
    DrawRectangle(x, y, cellSize, cellSize, DARKGRAY);
    DrawRectangleLines(x, y, cellSize, cellSize, GRAY);
    DrawCircle(x + cellSize/2, y + cellSize/2, 1, LIGHTGRAY);
}

void RenderManager::drawPlayer(const Player& player, bool hasSpeedBoost, 
                              bool isDigging) {
    Coordinate pos = player.getPosition();
    int screenX = pos.col * cellSize + cellSize / 2;
    int screenY = pos.row * cellSize + cellSize / 2;
    
    Color playerColor = YELLOW;
    
    if (hasSpeedBoost) {
        float pulse = AnimationSystem::pulse(8.0f);
        DrawCircleLines(screenX, screenY, 20, ColorAlpha(YELLOW, pulse));
    }
    
    float scale = 1.0f;
    if (isDigging) {
        scale = 1.0f + AnimationSystem::pulse(10.0f) * 0.15f;
    }
    
    int radius = static_cast<int>(16 * scale);
    DrawCircle(screenX, screenY, radius, playerColor);
    DrawCircleLines(screenX, screenY, radius, ORANGE);
    DrawCircle(screenX - 6, screenY - 6, 3, BLACK);
    DrawCircle(screenX + 6, screenY - 6, 3, BLACK);
}

void RenderManager::drawEnemies(const std::vector<Enemy>& enemies) {
    for (const auto& enemy : enemies) {
        if (!enemy.isActive()) continue;
        
        Coordinate pos = enemy.getPosition();
        int screenX = pos.col * cellSize + cellSize / 2;
        int screenY = pos.row * cellSize + cellSize / 2;
        
        if (enemy.getIsDestroyed()) {
            drawDestroyedEnemy(screenX, screenY, enemy.getDestroyProgress());
        } else {
            drawActiveEnemy(screenX, screenY, enemy);
        }
    }
}

void RenderManager::drawDestroyedEnemy(int x, int y, float progress) {
    float scale = 1.0f + AnimationSystem::easeOut(progress) * 2.0f;
    float alpha = 1.0f - AnimationSystem::easeIn(progress);
    
    Color destroyColor = Color{255, 255, 0, 
                             static_cast<unsigned char>(255 * alpha)};
    
    for (int i = 0; i < 3; ++i) {
        float circleScale = scale + i * 0.3f;
        int radius = static_cast<int>(15 * circleScale);
        DrawCircleLines(x, y, radius, destroyColor);
    }
    
    for (int i = 0; i < 8; ++i) {
        float angle = (i * 45.0f) * DEG2RAD;
        float distance = AnimationSystem::easeOut(progress) * 30;
        int px = x + static_cast<int>(cos(angle) * distance);
        int py = y + static_cast<int>(sin(angle) * distance);
        DrawCircle(px, py, 2, destroyColor);
    }
}

void RenderManager::drawActiveEnemy(int x, int y, const Enemy& enemy) {
    Color color;
    
    if (enemy.getEnemyType() == EnemyType::GREEN_DRAGON) {
        color = GREEN;
    } else if (enemy.getEnemyType() == EnemyType::AGGRESSIVE_MONSTER) {
        color = Color{139, 0, 0, 255};
    } else {
        color = RED;
    }
    
    if (enemy.getIsPhasing()) {
        float pulse = AnimationSystem::pulse(6.0f);
        color = ColorAlpha(color, 0.4f + pulse * 0.3f);
    }
    
    DrawCircle(x, y, 15, color);
    DrawCircleLines(x, y, 15, MAROON);
    DrawCircle(x - 5, y - 5, 2, BLACK);
    DrawCircle(x + 5, y - 5, 2, BLACK);
    
    if (enemy.getEnemyType() == EnemyType::GREEN_DRAGON) {
        DrawCircle(x, y + 8, 3, ORANGE);
    }
}

void RenderManager::drawHarpoons(const std::vector<Harpoon>& harpoons, 
                                bool hasPowerShot) {
    for (const auto& harpoon : harpoons) {
        if (!harpoon.isActive()) continue;
        
        const auto& segments = harpoon.getSegments();
        float pulse = AnimationSystem::pulse(8.0f);
        
        Color color = hasPowerShot ? 
            AnimationSystem::lerpColor(ORANGE, RED, pulse * 0.3f) :
            Color{50, 255, 50, 255};
        
        for (size_t i = 0; i < segments.size(); ++i) {
            Coordinate pos = segments[i];
            int screenX = pos.col * cellSize + cellSize / 2;
            int screenY = pos.row * cellSize + cellSize / 2;
            
            if (i == segments.size() - 1) {
                int tipSize = hasPowerShot ? 7 : 5;
                float scale = 1.0f + pulse * 0.2f;
                DrawCircle(screenX, screenY, static_cast<int>(tipSize * scale), color);
                DrawCircleLines(screenX, screenY, tipSize + 2, WHITE);
            } else {
                DrawCircle(screenX, screenY, i == 0 ? 4 : 3, color);
            }
            
            if (i > 0) {
                Coordinate prevPos = segments[i-1];
                int prevX = prevPos.col * cellSize + cellSize / 2;
                int prevY = prevPos.row * cellSize + cellSize / 2;
                DrawLine(prevX, prevY, screenX, screenY, color);
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
        
        float pulse = AnimationSystem::pulse(5.0f);
        float bounce = AnimationSystem::wave(3.0f) * 3;
        
        Color color = getPowerUpColor(powerUp.getType());
        color = ColorAlpha(color, 0.7f + pulse * 0.3f);
        
        DrawCircle(screenX, screenY - static_cast<int>(bounce), 12, color);
        DrawCircleLines(screenX, screenY - static_cast<int>(bounce), 14, WHITE);
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
            float pulse = AnimationSystem::pulse(10.0f);
            rockColor = AnimationSystem::lerpColor(PURPLE, RED, pulse);
            
            DrawCircle(screenX, screenY - 5, 16, ColorAlpha(RED, 0.3f));
        }
        
        DrawCircle(screenX, screenY, 18, rockColor);
        DrawCircleLines(screenX, screenY, 18, WHITE);
        DrawText("ROCK", screenX - 15, screenY - 6, 12, WHITE);
    }
}

void RenderManager::drawFireProjectiles(const std::vector<FireProjectile>& fires) {
    for (const auto& fire : fires) {
        if (!fire.isActive()) continue;
        
        const auto& trail = fire.getTrail();
        float pulse = AnimationSystem::pulse(15.0f);
        
        for (size_t i = 0; i < trail.size(); ++i) {
            Coordinate pos = trail[i];
            int screenX = pos.col * cellSize + cellSize / 2;
            int screenY = pos.row * cellSize + cellSize / 2;
            
            float alpha = (i + 1.0f) / trail.size();
            Color fireColor = AnimationSystem::lerpColor(ORANGE, RED, pulse);
            fireColor = ColorAlpha(fireColor, alpha);
            
            int size = (i == trail.size() - 1) ? 8 : 5;
            DrawCircle(screenX, screenY, size, fireColor);
            
            if (i == trail.size() - 1) {
                DrawCircleLines(screenX, screenY, size + 2, YELLOW);
            }
        }
    }
}

Color RenderManager::getPowerUpColor(PowerUpType type) {
    switch (type) {
        case PowerUpType::EXTRA_LIFE: return GREEN;
        case PowerUpType::SCORE_MULTIPLIER: return GOLD;
        case PowerUpType::RAPID_FIRE: return Color{0, 255, 255, 255};
        case PowerUpType::POWER_SHOT: return ORANGE;
        case PowerUpType::SPEED_BOOST: return YELLOW;
        default: return WHITE;
    }
}
