#include "Coordinate.h"
#include "BlockGrid.h"
#include "EnemyLogic.h"
#include "DynamicsEngine.h"
#include "GameObject.h"
#include <iostream>
#include <vector>

class GameManager {
private:
    BlockGrid terrain;
    DynamicsEngine physics;
    std::vector<GameObject*> gameObjects;
    bool gameRunning;
    int currentLevel;
    int playerLives;
    int score;

public:
    GameManager() : gameRunning(true), currentLevel(1), playerLives(3), score(0) {
        std::cout << "GameManager initialized" << std::endl;
    }
    
    ~GameManager() {
        cleanup();
    }
    
    void initializeGame() {
        terrain.initializeDefaultMap();
        std::cout << "Game initialized - Level " << currentLevel << std::endl;
        std::cout << "Lives: " << playerLives << " | Score: " << score << std::endl;
    }
    
    void update() {
        if (!gameRunning) return;
        
        for (auto* obj : gameObjects) {
            if (obj && obj->isActive()) {
                obj->update();
            }
        }
        
        physics.processAllInteractions(gameObjects);
    }
    
    void addGameObject(GameObject* obj) {
        if (obj) {
            gameObjects.push_back(obj);
        }
    }
    
    BlockGrid& getTerrain() { return terrain; }
    DynamicsEngine& getPhysics() { return physics; }
    bool isGameRunning() const { return gameRunning; }
    int getScore() const { return score; }
    int getLives() const { return playerLives; }
    
private:
    void cleanup() {
        for (auto* obj : gameObjects) {
            delete obj;
        }
        gameObjects.clear();
    }
};
