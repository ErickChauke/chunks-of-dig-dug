#ifndef COLLISIONMANAGER_H
#define COLLISIONMANAGER_H

#include "Player.h"
#include "Enemy.h"
#include "Harpoon.h"
#include "PowerUp.h"
#include "Rock.h"
#include <vector>

class CollisionManager {
public:
    CollisionManager() = default;
    
    bool checkPlayerEnemyCollision(const Player& player, 
                                  const std::vector<Enemy>& enemies);
    
    void checkHarpoonEnemyCollisions(std::vector<Harpoon>& harpoons, 
                                   std::vector<Enemy>& enemies, int& score, 
                                   int& enemiesDefeated, int level);
    
    void checkRockCollisions(std::vector<Rock>& rocks, const Player& player, 
                           std::vector<Enemy>& enemies, bool& playerCrushed);
    
    PowerUp* checkPowerUpCollision(const Player& player, 
                                  std::vector<PowerUp>& powerUps);

private:
    int getScoreForEnemy(EnemyType type, int level);
};

#endif // COLLISIONMANAGER_H
