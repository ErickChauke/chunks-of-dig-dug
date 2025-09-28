#ifndef LEVELMANAGER_H
#define LEVELMANAGER_H

#include "BlockGrid.h"
#include "Player.h"
#include "Enemy.h"
#include "PowerUp.h"
#include "Rock.h"
#include "Coordinate.h"
#include <vector>

class LevelManager {
private:
    int currentLevel;
    int targetScore;
    float nextPowerUpTime;

public:
    LevelManager();
    
    void initializeLevel(int level, BlockGrid& terrain, Player& player, 
                        std::vector<Enemy>& enemies, std::vector<PowerUp>& powerUps,
                        std::vector<Rock>& rocks);
    
    bool shouldSpawnPowerUp(float levelTimer) const;
    PowerUp createRandomPowerUp();
    void updatePowerUpSpawnTime(float levelTimer);
    
    bool isLevelComplete(const std::vector<Enemy>& enemies, int score) const;
    int calculateTimeBonus(float levelTimer, int level) const;
    
    void nextLevel();
    void reset();
    
    int getCurrentLevel() const { return currentLevel; }
    int getTargetScore() const { return targetScore; }

private:
    void spawnEnemies(std::vector<Enemy>& enemies);
    void spawnRocks(std::vector<Rock>& rocks, const BlockGrid& terrain);
    Coordinate findValidSpawnPosition(const Player& player) const;
    std::string getLevelMapFile(int level) const;
};

#endif // LEVELMANAGER_H
