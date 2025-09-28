#include "Coordinate.h"
#include "BlockGrid.h"
#include "EnemyLogic.h"
#include "DynamicsEngine.h"
#include "GameObject.h"
#include "Player.h"
#include "Enemy.h"
#include <iostream>
#include <vector>

enum class GameState {
    MENU,
    PLAYING,
    PAUSED,
    GAME_OVER,
    LEVEL_COMPLETE,
    VICTORY
};

/**
 * @brief GameManager class handles overall game state and coordination
 */
class GameManager {
private:
    BlockGrid terrain;
    DynamicsEngine physics;
    std::vector<GameObject*> gameObjects;
    GameState currentState;
    int currentLevel;
    int playerLives;
    int score;
    int targetScore;
    float levelTimer;
    float bonusMultiplier;
    bool levelComplete;

public:
    GameManager() : currentState(GameState::MENU), currentLevel(1), 
                   playerLives(3), score(0), targetScore(1000),
                   levelTimer(0.0f), bonusMultiplier(1.0f), levelComplete(false) {
        std::cout << "GameManager initialized for Sprint 5" << std::endl;
    }
    
    ~GameManager() {
        cleanup();
    }
    
    void initializeGame() {
        terrain.initializeDefaultMap();
        currentState = GameState::PLAYING;
        levelTimer = 0.0f;
        bonusMultiplier = 1.0f + (currentLevel - 1) * 0.2f;
        
        std::cout << "Level " << currentLevel << " initialized" << std::endl;
        std::cout << "Lives: " << playerLives << " | Target Score: " << targetScore << std::endl;
    }
    
    void update(float deltaTime) {
        switch (currentState) {
            case GameState::PLAYING:
                updatePlaying(deltaTime);
                break;
            case GameState::PAUSED:
                // Game is paused, don't update game objects
                break;
            case GameState::GAME_OVER:
            case GameState::LEVEL_COMPLETE:
            case GameState::VICTORY:
            case GameState::MENU:
                // Handle state-specific updates
                break;
        }
    }
    
    void updatePlaying(float deltaTime) {
        levelTimer += deltaTime;
        
        // Update all game objects
        for (auto* obj : gameObjects) {
            if (obj && obj->isActive()) {
                obj->update();
            }
        }
        
        // Process physics interactions
        physics.processAllInteractions(gameObjects);
        
        // Check win/lose conditions
        checkWinConditions();
        checkLoseConditions();
        
        // Remove inactive objects
        removeInactiveObjects();
    }
    
    void checkWinConditions() {
        // Level complete if score target reached or all enemies defeated
        if (score >= targetScore || allEnemiesDefeated()) {
            completeLevel();
        }
    }
    
    void checkLoseConditions() {
        // Game over if no lives left
        if (playerLives <= 0) {
            currentState = GameState::GAME_OVER;
            std::cout << "Game Over! Final Score: " << score << std::endl;
        }
    }
    
    void completeLevel() {
        if (!levelComplete) {
            levelComplete = true;
            currentState = GameState::LEVEL_COMPLETE;
            
            // Calculate time bonus
            int timeBonus = calculateTimeBonus();
            score += timeBonus;
            
            std::cout << "Level " << currentLevel << " Complete!" << std::endl;
            std::cout << "Time Bonus: " << timeBonus << std::endl;
            std::cout << "Total Score: " << score << std::endl;
            
            // Check for game victory
            if (currentLevel >= 10) {
                currentState = GameState::VICTORY;
                std::cout << "Congratulations! You've completed all levels!" << std::endl;
            }
        }
    }
    
    void nextLevel() {
        if (currentState == GameState::LEVEL_COMPLETE) {
            currentLevel++;
            targetScore += 1000 * currentLevel;
            levelComplete = false;
            levelTimer = 0.0f;
            
            // Clear objects for new level
            cleanup();
            
            initializeGame();
        }
    }
    
    void pauseGame() {
        if (currentState == GameState::PLAYING) {
            currentState = GameState::PAUSED;
            std::cout << "Game Paused" << std::endl;
        }
    }
    
    void resumeGame() {
        if (currentState == GameState::PAUSED) {
            currentState = GameState::PLAYING;
            std::cout << "Game Resumed" << std::endl;
        }
    }
    
    void restartGame() {
        currentLevel = 1;
        playerLives = 3;
        score = 0;
        targetScore = 1000;
        levelComplete = false;
        cleanup();
        initializeGame();
        std::cout << "Game Restarted" << std::endl;
    }
    
    void addGameObject(GameObject* obj) {
        if (obj) {
            gameObjects.push_back(obj);
        }
    }
    
    void removeInactiveObjects() {
        gameObjects.erase(
            std::remove_if(gameObjects.begin(), gameObjects.end(),
                [](GameObject* obj) { return !obj || !obj->isActive(); }),
            gameObjects.end()
        );
    }
    
    // Getters
    BlockGrid& getTerrain() { return terrain; }
    DynamicsEngine& getPhysics() { return physics; }
    GameState getCurrentState() const { return currentState; }
    int getCurrentLevel() const { return currentLevel; }
    int getScore() const { return score; }
    int getTargetScore() const { return targetScore; }
    int getLives() const { return playerLives; }
    float getLevelTimer() const { return levelTimer; }
    float getBonusMultiplier() const { return bonusMultiplier; }
    
    // Score management
    void addScore(int points) { 
        score += static_cast<int>(points * bonusMultiplier);
    }
    
    void loseLife() { 
        if (playerLives > 0) {
            playerLives--;
            std::cout << "Life lost! Lives remaining: " << playerLives << std::endl;
        }
    }
    
    void addExtraLife() {
        playerLives++;
        std::cout << "Extra life gained! Lives: " << playerLives << std::endl;
    }

private:
    bool allEnemiesDefeated() {
        for (auto* obj : gameObjects) {
            Enemy* enemy = dynamic_cast<Enemy*>(obj);
            if (enemy && enemy->isActive()) {
                return false;
            }
        }
        return true;
    }
    
    int calculateTimeBonus() {
        // Bonus decreases with time taken
        float maxTime = 180.0f; // 3 minutes
        float timeRatio = std::max(0.0f, (maxTime - levelTimer) / maxTime);
        return static_cast<int>(1000 * timeRatio * bonusMultiplier);
    }
    
    void cleanup() {
        for (auto* obj : gameObjects) {
            delete obj;
        }
        gameObjects.clear();
    }
};
