#include "CollisionManager.h"
#include "GameConstants.h"
#include <iostream>

using namespace GameConstants;

bool CollisionManager::checkPlayerEnemyCollision(const Player& player, 
                                                const std::vector<Enemy>& enemies) {
    if (!player.isActive()) return false;
    
    Coordinate playerPos = player.getPosition();
    Coordinate playerBounds = player.getCollisionBounds();
    
    for (const auto& enemy : enemies) {
        if (enemy.isActive() && !enemy.getIsDestroyed()) {
            Coordinate enemyPos = enemy.getPosition();
            Coordinate enemyBounds = enemy.getCollisionBounds();
            
            if (checkAABBCollision(playerPos, playerBounds, enemyPos, enemyBounds)) {
                return true;
            }
        }
    }
    return false;
}

void CollisionManager::checkHarpoonEnemyCollisions(std::vector<Harpoon>& harpoons, 
                                                  std::vector<Enemy>& enemies, 
                                                  int& score, int& enemiesDefeated, 
                                                  int level) {
    for (auto& harpoon : harpoons) {
        if (!harpoon.isActive()) continue;
        
        const auto& segments = harpoon.getSegments();
        
        for (auto& enemy : enemies) {
            if (!enemy.isActive() || enemy.getIsDestroyed()) continue;
            
            Coordinate enemyPos = enemy.getPosition();
            
            for (const auto& segment : segments) {
                if (isPositionMatch(segment, enemyPos)) {
                    enemy.destroy();
                    score += getScoreForEnemy(enemy.getEnemyType(), level);
                    enemiesDefeated++;
                    break;
                }
            }
        }
    }
}

void CollisionManager::checkRockCollisions(std::vector<Rock>& rocks, 
                                          const Player& player, 
                                          std::vector<Enemy>& enemies, 
                                          bool& playerCrushed) {
    playerCrushed = false;
    
    for (auto& rock : rocks) {
        if (!rock.isActive()) continue;
        
        rock.handleCrushingLogic(player, enemies);
        
        if (rock.checkPlayerCrush(player)) {
            playerCrushed = true;
            return;
        }
    }
}

PowerUp* CollisionManager::checkPowerUpCollision(const Player& player, 
                                               std::vector<PowerUp>& powerUps) {
    if (!player.isActive()) return nullptr;
    
    Coordinate playerPos = player.getPosition();
    Coordinate playerBounds = player.getCollisionBounds();
    
    for (auto& powerUp : powerUps) {
        if (powerUp.isActive() && !powerUp.isCollected()) {
            Coordinate powerUpPos = powerUp.getPosition();
            Coordinate powerUpBounds = powerUp.getCollisionBounds();
            
            if (checkAABBCollision(playerPos, playerBounds, 
                                  powerUpPos, powerUpBounds)) {
                return &powerUp;
            }
        }
    }
    return nullptr;
}

int CollisionManager::getScoreForEnemy(EnemyType type, int level) {
    int baseScore = (type == EnemyType::AGGRESSIVE_MONSTER) ? 200 : 100;
    return baseScore * level;
}

bool CollisionManager::checkAABBCollision(Coordinate pos1, Coordinate bounds1,
                                         Coordinate pos2, Coordinate bounds2) {
    return (pos1.row < pos2.row + bounds2.row &&
            pos1.row + bounds1.row > pos2.row &&
            pos1.col < pos2.col + bounds2.col &&
            pos1.col + bounds1.col > pos2.col);
}

bool CollisionManager::isPositionMatch(Coordinate pos1, Coordinate pos2) {
    return pos1.row == pos2.row && pos1.col == pos2.col;
}
