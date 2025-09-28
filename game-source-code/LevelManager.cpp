#include "LevelManager.h"
#include <iostream>
#include <cstdlib>

LevelManager::LevelManager() : currentLevel(1), targetScore(1000), 
                               nextPowerUpTime(15.0f) {
}

void LevelManager::initializeLevel(int level, BlockGrid& terrain, Player& player, 
                                  std::vector<Enemy>& enemies, 
                                  std::vector<PowerUp>& powerUps,
                                  std::vector<Rock>& rocks) {
    currentLevel = level;
    std::string mapFile = getLevelMapFile(level);
    terrain.importMapFromFile(mapFile);
    
    // Clear containers
    enemies.clear();
    powerUps.clear();
    rocks.clear();
    
    // Reset player
    player.reset(Coordinate(Coordinate::PLAYABLE_START_ROW, 1));
    
    // Now spawn entities
    spawnEnemies(enemies);
    spawnRocks(rocks, terrain);
    
    nextPowerUpTime = 15.0f;
}

bool LevelManager::shouldSpawnPowerUp(float levelTimer) const {
    return levelTimer >= nextPowerUpTime;
}

void LevelManager::updatePowerUpSpawnTime(float levelTimer) {
    nextPowerUpTime = levelTimer + 20.0f + (std::rand() % 10);
}

bool LevelManager::isLevelComplete(const std::vector<Enemy>& enemies, 
                                  int score) const {
    bool allEnemiesDefeated = true;
    for (const auto& enemy : enemies) {
        if (enemy.isActive() && !enemy.getIsDestroyed()) {
            allEnemiesDefeated = false;
            break;
        }
    }
    return allEnemiesDefeated || score >= targetScore;
}

int LevelManager::calculateTimeBonus(float levelTimer, int level) const {
    float maxTime = 180.0f;
    float timeRatio = std::max(0.0f, (maxTime - levelTimer) / maxTime);
    return static_cast<int>(1000 * timeRatio);
}

void LevelManager::nextLevel() {
    currentLevel++;
    targetScore += 1000 * currentLevel;
}

void LevelManager::reset() {
    currentLevel = 1;
    targetScore = 1000;
    nextPowerUpTime = 15.0f;
}

std::string LevelManager::getLevelMapFile(int level) const {
    return "resources/maps/level" + std::to_string(level) + ".txt";
}

void LevelManager::spawnEnemies(std::vector<Enemy>& enemies) {
    int numEnemies = 2 + currentLevel;
    
    for (int i = 0; i < numEnemies && i < 8; ++i) {
        Coordinate spawnPos = findValidSpawnPosition(Player());
        EnemyType type = (i % 3 == 0) ? EnemyType::AGGRESSIVE_MONSTER : 
                                       EnemyType::RED_MONSTER;
        enemies.emplace_back(spawnPos, type);
    }
}

void LevelManager::spawnRocks(std::vector<Rock>& rocks, const BlockGrid& terrain) {
    int numRocks = 4 + (currentLevel / 2);
    numRocks = std::min(numRocks, 8);
    
    for (int i = 0; i < numRocks; ++i) {
        int row = Coordinate::PLAYABLE_START_ROW + 3 + (std::rand() % 8);
        int col = 5 + (std::rand() % (Coordinate::WORLD_COLS - 10));
        rocks.emplace_back(Coordinate(row, col));
    }
}

Coordinate LevelManager::findValidSpawnPosition(const Player& player) const {
    int row = Coordinate::PLAYABLE_START_ROW + 2 + 
             (std::rand() % (Coordinate::PLAYABLE_ROWS - 5));
    int col = 5 + (std::rand() % (Coordinate::WORLD_COLS - 10));
    return Coordinate(row, col);
}

PowerUp LevelManager::createRandomPowerUp() {
    Coordinate pos = findValidSpawnPosition(Player());
    PowerUpType types[] = {
        PowerUpType::EXTRA_LIFE,
        PowerUpType::SCORE_MULTIPLIER,
        PowerUpType::RAPID_FIRE,
        PowerUpType::POWER_SHOT,
        PowerUpType::SPEED_BOOST
    };
    PowerUpType selectedType = types[std::rand() % 5];
    return PowerUp(pos, selectedType);
}
