#include "CollisionManager.h"
#include <iostream>

bool CollisionManager::checkPlayerEnemyCollision(const Player& player, 
                                                const std::vector<Enemy>& enemies) {
    if (!player.isActive()) return false;
    
    for (const auto& enemy : enemies) {
        if (enemy.isActive() && !enemy.getIsDestroyed()) {
            if (enemy.getPosition() == player.getPosition()) {
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
        
        for (auto& enemy : enemies) {
            if (enemy.isActive() && !enemy.getIsDestroyed() && 
                harpoon.checkEnemyHit(enemy.getPosition())) {
                enemy.destroy();
                score += getScoreForEnemy(enemy.getEnemyType(), level);
                enemiesDefeated++;
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
        
        // Handle enemy crushing first
        rock.handleCrushingLogic(player, enemies);
        
        // Check if player gets crushed
        if (rock.checkPlayerCrush(player)) {
            std::cout << "PLAYER CRUSHED by rock at (" << rock.getPosition().row 
                      << "," << rock.getPosition().col << ")!" << std::endl;
            playerCrushed = true;
            return; // Exit immediately when player is crushed
        }
    }
}

PowerUp* CollisionManager::checkPowerUpCollision(const Player& player, 
                                               std::vector<PowerUp>& powerUps) {
    if (!player.isActive()) return nullptr;
    
    for (auto& powerUp : powerUps) {
        if (powerUp.isActive() && powerUp.getPosition() == player.getPosition()) {
            return &powerUp;
        }
    }
    return nullptr;
}

int CollisionManager::getScoreForEnemy(EnemyType type, int level) {
    int baseScore = (type == EnemyType::AGGRESSIVE_MONSTER) ? 200 : 100;
    return baseScore * level;
}
