#include <raylib-cpp.hpp>
#include <iostream>
#include <vector>
#include "Coordinate.h"
#include "BlockGrid.h"
#include "Player.h"
#include "Enemy.h"
#include "Harpoon.h"
#include "PowerUp.h"
#include "Rock.h"
#include "RenderManager.h"
#include "UIManager.h"
#include "InputManager.h"
#include "CollisionManager.h"
#include "LevelManager.h"
#include "PowerUpManager.h"
#include "GameState.h"

const int SCREEN_WIDTH = 1200;
const int SCREEN_HEIGHT = 800;
const int CELL_SIZE = 40;

/**
 * @brief Main game class following single responsibility principle
 */
class DigDugGame {
private:
    // Core systems
    raylib::Window window;
    RenderManager renderer;
    UIManager uiManager;
    InputManager inputManager;
    CollisionManager collisionManager;
    LevelManager levelManager;
    PowerUpManager powerUpManager;
    GameStateManager stateManager;
    
    // Game world
    BlockGrid terrain;
    Player player;
    std::vector<Enemy> enemies;
    std::vector<Harpoon> harpoons;
    std::vector<PowerUp> powerUps;
    std::vector<Rock> rocks;
    
    // Game state
    int score;
    int enemiesDefeated;
    int playerLives;
    float lastHarpoonTime;
    float levelTimer;

public:
    DigDugGame() : window(SCREEN_WIDTH, SCREEN_HEIGHT, "Underground Adventure"),
                   renderer(CELL_SIZE, SCREEN_WIDTH, SCREEN_HEIGHT),
                   uiManager(SCREEN_WIDTH, SCREEN_HEIGHT, CELL_SIZE),
                   player(Coordinate(Coordinate::PLAYABLE_START_ROW, 1)),
                   score(0), enemiesDefeated(0), playerLives(3), 
                   lastHarpoonTime(0.0f), levelTimer(0.0f) {
        window.SetTargetFPS(60);
        initializeNewGame();
    }
    
    void run() {
        while (!window.ShouldClose()) {
            update();
            render();
        }
    }

private:
    void update() {
        float deltaTime = GetFrameTime();
        
        switch (stateManager.getCurrentState()) {
            case GameState::MENU:
                handleMenuState();
                break;
            case GameState::PLAYING:
                updateGameplay(deltaTime);
                break;
            case GameState::PAUSED:
                handlePauseState();
                break;
            case GameState::GAME_OVER:
            case GameState::LEVEL_COMPLETE:
            case GameState::VICTORY:
                handleEndGameState();
                break;
            default:
                break;
        }
        
        stateManager.update(deltaTime);
    }
    
    void updateGameplay(float deltaTime) {
        levelTimer += deltaTime;
        
        handleGameInput();
        updateGameObjects();
        powerUpManager.update();
        checkAllCollisions();
        checkLevelProgression();
        spawnPowerUps();
    }
    
    void updateGameObjects() {
        player.update();
        
        for (auto& enemy : enemies) {
            if (enemy.isActive()) {
                if (!enemy.getIsDestroyed()) {
                    enemy.moveToward(player.getPosition(), terrain);
                }
                enemy.update();
            }
        }
        
        harpoons.erase(
            std::remove_if(harpoons.begin(), harpoons.end(),
                [](Harpoon& h) { 
                    if (h.isActive()) {
                        h.update();
                        return false;
                    }
                    return true;
                }),
            harpoons.end()
        );
        
        powerUps.erase(
            std::remove_if(powerUps.begin(), powerUps.end(),
                [](PowerUp& p) {
                    if (p.isActive()) {
                        p.update();
                        return false;
                    }
                    return true;
                }),
            powerUps.end()
        );
        
        for (auto& rock : rocks) {
            if (rock.isActive()) {
                rock.update();
                rock.applyGravity(terrain);
            }
        }
    }
    
    void handleGameInput() {
        player.handleMovementWithRocks(terrain, rocks);
        
        if (inputManager.isHarpoonPressed() && canFireHarpoon()) {
            fireHarpoon();
        }
        
        if (inputManager.isPausePressed()) {
            stateManager.changeState(GameState::PAUSED);
        }
    }
    
    void fireHarpoon() {
        Direction playerDir = player.getLastMoveDirection();
        if (playerDir != Direction::NONE) {
            harpoons.emplace_back(player.getPosition(), playerDir, &player);
            lastHarpoonTime = GetTime();
        }
    }
    
    bool canFireHarpoon() {
        return (GetTime() - lastHarpoonTime) >= powerUpManager.getHarpoonCooldown();
    }
    
    void checkAllCollisions() {
        if (collisionManager.checkPlayerEnemyCollision(player, enemies)) {
            playerLives--;
            if (playerLives > 0) {
                player.reset(Coordinate(Coordinate::PLAYABLE_START_ROW, 1));
            } else {
                stateManager.changeState(GameState::GAME_OVER);
            }
        }
        
        int enemiesDefeated = 0;
        collisionManager.checkHarpoonEnemyCollisions(harpoons, enemies, score, 
                                                   enemiesDefeated, 1);
    }
    
    void handleMenuState() {
        if (inputManager.getMenuInput() == InputAction::CONFIRM) {
            stateManager.changeState(GameState::PLAYING);
        }
    }
    
    void handlePauseState() {
        if (inputManager.getPauseInput() == InputAction::CONFIRM) {
            stateManager.changeState(GameState::PLAYING);
        }
    }
    
    void handleEndGameState() {
        // Basic end game handling - will be enhanced in next commit
    }
    
    void initializeNewGame() {
        terrain.initializeDefaultMap();
        score = 0;
        playerLives = 3;
        levelTimer = 0.0f;
    }
    
    void initializeLevel() {
        // Basic level initialization - will be enhanced in next commit
    }
    
    void checkLevelProgression() {
        // Basic level progression - will be enhanced in next commit
    }
    
    void spawnPowerUps() {
        // Basic power-up spawning - will be enhanced in next commit
    }
    
    void render() {
        BeginDrawing();
        ClearBackground(BLACK);
        
        switch (stateManager.getCurrentState()) {
            case GameState::MENU:
                uiManager.drawMenu();
                break;
            case GameState::PLAYING:
            case GameState::PAUSED:
                drawGameScene();
                if (stateManager.getCurrentState() == GameState::PAUSED) {
                    uiManager.drawPauseOverlay();
                }
                break;
            case GameState::GAME_OVER:
                drawGameScene();
                uiManager.drawGameOverScreen(score, 1);
                break;
        }
        
        EndDrawing();
    }
    
    void drawGameScene() {
        std::vector<PowerUpEffect> emptyEffects;
        uiManager.drawHUD(1, score, 1000, playerLives, levelTimer, 
                         emptyEffects, canFireHarpoon(), 1.0f);
        
        renderer.drawTerrain(terrain);
        renderer.drawPlayer(player, false, player.getIsDigging());
        renderer.drawEnemies(enemies);
        renderer.drawHarpoons(harpoons, false);
        renderer.drawRocks(rocks, player);
    }
};

int main() {
    try {
        DigDugGame game;
        game.run();
    } catch (const std::exception& e) {
        std::cout << "Error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}
