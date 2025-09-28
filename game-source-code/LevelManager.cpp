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
    
    for (int i = 0; i < numEnemies && i < 8; ++i) {
        Coordinate spawnPos = findValidSpawnPosition(
            Player(Coordinate(Coordinate::PLAYABLE_START_ROW, 1)));
        
        EnemyType type = (i % 3 == 0) ? EnemyType::AGGRESSIVE_MONSTER : 
                                       EnemyType::RED_MONSTER;
        enemies.emplace_back(spawnPos, type);
    }
}

void LevelManager::spawnRocks(std::vector<Rock>& rocks, const BlockGrid& terrain) {
    std::vector<Coordinate> rockSpawns = terrain.getRockSpawns();
    
    if (rockSpawns.empty()) {
        std::cout << "No rock spawns in map, generating distributed rock positions" << std::endl;
        
        // Create more rocks distributed across the level
        int numRocks = 4 + (currentLevel / 2); // More rocks in higher levels
        numRocks = std::min(numRocks, 10); // Cap at 10 rocks maximum
        
        for (int i = 0; i < numRocks; ++i) {
            // Distribute rocks across different areas of the map
            int sectionWidth = Coordinate::WORLD_COLS / 3;
            int section = i % 3; // Distribute across left, middle, right
            
            int col = (section * sectionWidth) + 3 + (std::rand() % (sectionWidth - 6));
            int row = Coordinate::PLAYABLE_START_ROW + 3 + (std::rand() % 8);
            
            // Ensure rocks don't spawn too close to player start
            if (col < 8 && row < Coordinate::PLAYABLE_START_ROW + 5) {
                col += 8;
            }
            
            rocks.emplace_back(Coordinate(row, col));
            std::cout << "  Generated rock at (" << row << "," << col << ")" << std::endl;
        }
    } else {
        std::cout << "Spawning rocks from map positions:" << std::endl;
        for (const auto& rockPos : rockSpawns) {
            rocks.emplace_back(rockPos);
            std::cout << "  Rock at (" << rockPos.row << "," << rockPos.col << ")" << std::endl;
        }
        
        // Add extra generated rocks to supplement map rocks
        int extraRocks = std::max(0, (4 + currentLevel / 2) - static_cast<int>(rockSpawns.size()));
        for (int i = 0; i < extraRocks; ++i) {
            Coordinate pos = findValidRockPosition(rockSpawns);
            rocks.emplace_back(pos);
            std::cout << "  Extra rock at (" << pos.row << "," << pos.col << ")" << std::endl;
        }
    }
    
    std::cout << "Total rocks spawned: " << rocks.size() << std::endl;
}

Coordinate LevelManager::findValidRockPosition(const std::vector<Coordinate>& existingRocks) const {
    for (int attempts = 0; attempts < 50; ++attempts) {
        int row = Coordinate::PLAYABLE_START_ROW + 2 + (std::rand() % 10);
        int col = 5 + (std::rand() % (Coordinate::WORLD_COLS - 10));
        Coordinate pos(row, col);
        
        // Check distance from existing rocks
        bool tooClose = false;
        for (const auto& existing : existingRocks) {
            if (pos.calculateDistance(existing) < 4.0f) {
                tooClose = true;
                break;
            }
        }
        
        // Check distance from player start
        Coordinate playerStart(Coordinate::PLAYABLE_START_ROW, 1);
        if (!tooClose && pos.calculateDistance(playerStart) > 8.0f) {
            return pos;
        }
    }
    
    // Fallback position
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
    float maxTime = 180.0f; // 3 minutes
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
    return Coordinate(15, 20); // Fallback position
}

std::string LevelManager::getLevelMapFile(int level) const {
    return "resources/maps/level" + std::to_string(level) + ".txt";
}
