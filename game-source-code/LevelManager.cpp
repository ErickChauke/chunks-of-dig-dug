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
    
    enemies.clear();
    powerUps.clear();
    rocks.clear();
    
    player.reset(Coordinate(Coordinate::PLAYABLE_START_ROW, 1));
    
    spawnEnemies(enemies);
    spawnRocks(rocks, terrain);
    
    nextPowerUpTime = 15.0f;
    
    std::cout << "Level " << level << " initialized" << std::endl;
}

void LevelManager::spawnEnemies(std::vector<Enemy>& enemies) {
    int numEnemies = 2 + currentLevel;
    int numGreenDragons = (currentLevel >= 3) ? (currentLevel / 3) : 0;
    
    for (int i = 0; i < numEnemies && i < 8; ++i) {
        Coordinate spawnPos = findValidSpawnPosition(
            Player(Coordinate(Coordinate::PLAYABLE_START_ROW, 1)));
        
        EnemyType type;
        if (i < numGreenDragons) {
            type = EnemyType::GREEN_DRAGON;
        } else if (i % 3 == 0) {
            type = EnemyType::AGGRESSIVE_MONSTER;
        } else {
            type = EnemyType::RED_MONSTER;
        }
        
        enemies.emplace_back(spawnPos, type);
    }
    
    std::cout << "Spawned " << enemies.size() << " enemies (" 
              << numGreenDragons << " green dragons)" << std::endl;
}

void LevelManager::spawnRocks(std::vector<Rock>& rocks, const BlockGrid& terrain) {
    std::vector<Coordinate> rockSpawns = terrain.getRockSpawns();
    
    if (rockSpawns.empty()) {
        int numRocks = 4 + (currentLevel / 2);
        numRocks = std::min(numRocks, 10);
        
        for (int i = 0; i < numRocks; ++i) {
            int sectionWidth = Coordinate::WORLD_COLS / 3;
            int section = i % 3;
            
            int col = (section * sectionWidth) + 3 + (std::rand() % (sectionWidth - 6));
            int row = Coordinate::PLAYABLE_START_ROW + 3 + (std::rand() % 8);
            
            if (col < 8 && row < Coordinate::PLAYABLE_START_ROW + 5) {
                col += 8;
            }
            
            rocks.emplace_back(Coordinate(row, col));
        }
    } else {
        for (const auto& rockPos : rockSpawns) {
            rocks.emplace_back(rockPos);
        }
        
        int extraRocks = std::max(0, (4 + currentLevel / 2) - static_cast<int>(rockSpawns.size()));
        for (int i = 0; i < extraRocks; ++i) {
            Coordinate pos = findValidRockPosition(rockSpawns);
            rocks.emplace_back(pos);
        }
    }
}

Coordinate LevelManager::findValidRockPosition(const std::vector<Coordinate>& existingRocks) const {
    for (int attempts = 0; attempts < 50; ++attempts) {
        int row = Coordinate::PLAYABLE_START_ROW + 2 + (std::rand() % 10);
        int col = 5 + (std::rand() % (Coordinate::WORLD_COLS - 10));
        Coordinate pos(row, col);
        
        bool tooClose = false;
        for (const auto& existing : existingRocks) {
            if (pos.calculateDistance(existing) < 4.0f) {
                tooClose = true;
                break;
            }
        }
        
        Coordinate playerStart(Coordinate::PLAYABLE_START_ROW, 1);
        if (!tooClose && pos.calculateDistance(playerStart) > 8.0f) {
            return pos;
        }
    }
    
    return Coordinate(12, 15);
}

bool LevelManager::shouldSpawnPowerUp(float levelTimer) const {
    return levelTimer >= nextPowerUpTime;
}

PowerUp LevelManager::createRandomPowerUp() {
    Coordinate spawnPos = findValidSpawnPosition(
        Player(Coordinate(Coordinate::PLAYABLE_START_ROW, 1)));
    
    PowerUpType types[] = {
        PowerUpType::EXTRA_LIFE,
        PowerUpType::SCORE_MULTIPLIER,
        PowerUpType::RAPID_FIRE,
        PowerUpType::POWER_SHOT,
        PowerUpType::SPEED_BOOST
    };
    
    PowerUpType selectedType = types[std::rand() % 5];
    return PowerUp(spawnPos, selectedType);
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
    float bonusMultiplier = 1.0f + (level - 1) * 0.2f;
    return static_cast<int>(1000 * timeRatio * bonusMultiplier);
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

Coordinate LevelManager::findValidSpawnPosition(const Player& player) const {
    for (int attempts = 0; attempts < 50; ++attempts) {
        int row = Coordinate::PLAYABLE_START_ROW + 2 + 
                 (std::rand() % (Coordinate::PLAYABLE_ROWS - 5));
        int col = 5 + (std::rand() % (Coordinate::WORLD_COLS - 10));
        Coordinate pos(row, col);
        
        if (pos.calculateDistance(player.getPosition()) > 6.0f) {
            return pos;
        }
    }
    return Coordinate(15, 20);
}

std::string LevelManager::getLevelMapFile(int level) const {
    return "resources/maps/level" + std::to_string(level) + ".txt";
}
