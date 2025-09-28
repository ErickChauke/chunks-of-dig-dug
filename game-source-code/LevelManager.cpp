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
    
    // TODO: Implement spawning in next commit
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
    return score >= targetScore; // Simple completion check for now
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

// Stub implementations (will be enhanced in next commit)
void LevelManager::spawnEnemies(std::vector<Enemy>& enemies) {
    // TODO: Implement in next commit
}

void LevelManager::spawnRocks(std::vector<Rock>& rocks, const BlockGrid& terrain) {
    // TODO: Implement in next commit
}

Coordinate LevelManager::findValidSpawnPosition(const Player& player) const {
    // Simple fallback position for now
    return Coordinate(Coordinate::PLAYABLE_START_ROW + 5, 10);
}

PowerUp LevelManager::createRandomPowerUp() {
    Coordinate pos = findValidSpawnPosition(Player());
    return PowerUp(pos, PowerUpType::EXTRA_LIFE);
}
