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
#include "FireProjectile.h"
#include "RenderManager.h"
#include "UIManager.h"
#include "InputManager.h"
#include "CollisionManager.h"
#include "LevelManager.h"
#include "PowerUpManager.h"
#include "GameState.h"
#include "ParticleSystem.h"
#include "ScreenShake.h"
#include "GameConstants.h"

using namespace GameConstants;

class DigDugGame {
private:
    raylib::Window window;
    RenderManager renderer;
    UIManager uiManager;
    InputManager inputManager;
    CollisionManager collisionManager;
    LevelManager levelManager;
    PowerUpManager powerUpManager;
    GameStateManager stateManager;
    ParticleSystem particles;
    ScreenShake screenShake;
    
    BlockGrid terrain;
    Player player;
    std::vector<Enemy> enemies;
    std::vector<Harpoon> harpoons;
    std::vector<PowerUp> powerUps;
    std::vector<Rock> rocks;
    std::vector<FireProjectile> fireProjectiles;
    
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
                   score(0), enemiesDefeated(0), playerLives(STARTING_LIVES), 
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
        particles.update();
        screenShake.update();
        
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
        
        if (player.getIsDigging()) {
            Coordinate pos = player.getPosition();
            Vector2 particlePos = {
                pos.col * CELL_SIZE + CELL_SIZE / 2.0f,
                pos.row * CELL_SIZE + CELL_SIZE / 2.0f
            };
            particles.emitTrail(particlePos, BROWN);
        }
    }
    
    void updateGameObjects() {
        player.update();
        updateEnemies();
        updateHarpoons();
        updatePowerUps();
        updateRocks();
        updateFireProjectiles();
    }
    
    void updateEnemies() {
        for (auto& enemy : enemies) {
            if (enemy.isActive()) {
                bool wasDestroyed = enemy.getIsDestroyed();
                
                if (!enemy.getIsDestroyed()) {
                    enemy.moveToward(player.getPosition(), terrain);
                    
                    if (enemy.shouldBreatheFire(player.getPosition())) {
                        Direction fireDir = enemy.getFireDirection(player.getPosition());
                        fireProjectiles.emplace_back(enemy.getPosition(), fireDir);
                        
                        Coordinate pos = enemy.getPosition();
                        Vector2 particlePos = {
                            pos.col * CELL_SIZE + CELL_SIZE / 2.0f,
                            pos.row * CELL_SIZE + CELL_SIZE / 2.0f
                        };
                        particles.emit(particlePos, ORANGE, 8);
                    }
                }
                enemy.update();
                
                if (!wasDestroyed && enemy.getIsDestroyed()) {
                    Coordinate pos = enemy.getPosition();
                    Vector2 particlePos = {
                        pos.col * CELL_SIZE + CELL_SIZE / 2.0f,
                        pos.row * CELL_SIZE + CELL_SIZE / 2.0f
                    };
                    particles.emitBurst(particlePos, YELLOW, 15);
                    screenShake.shake(5.0f, 0.2f);
                }
            }
        }
    }
    
    void updateHarpoons() {
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
    }
    
    void updatePowerUps() {
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
    }
    
    void updateRocks() {
        for (auto& rock : rocks) {
            if (rock.isActive()) {
                bool wasFalling = rock.getIsFalling();
                rock.update();
                rock.applyGravity(terrain);
                
                if (wasFalling && rock.getHasLanded()) {
                    Coordinate pos = rock.getPosition();
                    Vector2 particlePos = {
                        pos.col * CELL_SIZE + CELL_SIZE / 2.0f,
                        pos.row * CELL_SIZE + CELL_SIZE / 2.0f
                    };
                    particles.emit(particlePos, GRAY, 10);
                    screenShake.shake(8.0f, 0.25f);
                }
            }
        }
    }
    
    void updateFireProjectiles() {
        for (auto& fire : fireProjectiles) {
            if (fire.isActive()) {
                fire.update();
                
                if (fire.checkPlayerHit(player.getPosition())) {
                    playerHitByFire();
                    fire.setActive(false);
                }
            }
        }
        
        fireProjectiles.erase(
            std::remove_if(fireProjectiles.begin(), fireProjectiles.end(),
                [](const FireProjectile& f) { return !f.isActive(); }),
            fireProjectiles.end()
        );
    }
    
    void handleGameInput() {
        player.handleMovementWithRocks(terrain, rocks);
        
        if (inputManager.isHarpoonPressed() && canFireHarpoon()) {
            fireHarpoon();
        }
        
        if (inputManager.isPausePressed()) {
            stateManager.changeState(GameState::PAUSED);
        }
        
        if (inputManager.isRestartPressed()) {
            restartGame();
        }
        
        if (inputManager.isExitPressed()) {
            window.Close();
        }
    }
    
    void fireHarpoon() {
        Direction playerDir = player.getLastMoveDirection();
        if (playerDir != Direction::NONE) {
            Coordinate playerPos = player.getPosition();
            harpoons.emplace_back(playerPos, playerDir, &player);
            lastHarpoonTime = GetTime();
        }
    }
    
    bool canFireHarpoon() {
        float currentTime = GetTime();
        return (currentTime - lastHarpoonTime) >= powerUpManager.getHarpoonCooldown();
    }
    
    void checkAllCollisions() {
        if (collisionManager.checkPlayerEnemyCollision(player, enemies)) {
            playerHit();
            return;
        }
        
        collisionManager.checkHarpoonEnemyCollisions(harpoons, enemies, score, 
                                                   enemiesDefeated, 
                                                   levelManager.getCurrentLevel());
        
        bool playerCrushed = false;
        collisionManager.checkRockCollisions(rocks, player, enemies, playerCrushed);
        if (playerCrushed) {
            playerHit();
            return;
        }
        
        PowerUp* collectedPowerUp = collisionManager.checkPowerUpCollision(player, 
                                                                          powerUps);
        if (collectedPowerUp) {
            Coordinate pos = collectedPowerUp->getPosition();
            Vector2 particlePos = {
                pos.col * CELL_SIZE + CELL_SIZE / 2.0f,
                pos.row * CELL_SIZE + CELL_SIZE / 2.0f
            };
            particles.emit(particlePos, GOLD, 12);
            
            powerUpManager.collectPowerUp(*collectedPowerUp, player, 
                                        playerLives, score);
            collectedPowerUp->collect();
        }
    }
    
    void playerHit() {
        Coordinate pos = player.getPosition();
        Vector2 particlePos = {
            pos.col * CELL_SIZE + CELL_SIZE / 2.0f,
            pos.row * CELL_SIZE + CELL_SIZE / 2.0f
        };
        particles.emitBurst(particlePos, RED, 20);
        screenShake.shake(15.0f, 0.4f);
        
        playerLives--;
        
        if (playerLives <= 0) {
            stateManager.changeState(GameState::GAME_OVER);
        } else {
            player.reset(Coordinate(Coordinate::PLAYABLE_START_ROW, 1));
        }
    }
    
    void playerHitByFire() {
        Coordinate pos = player.getPosition();
        Vector2 particlePos = {
            pos.col * CELL_SIZE + CELL_SIZE / 2.0f,
            pos.row * CELL_SIZE + CELL_SIZE / 2.0f
        };
        particles.emitBurst(particlePos, ORANGE, 15);
        screenShake.shake(12.0f, 0.3f);
        
        playerLives--;
        
        if (playerLives <= 0) {
            stateManager.changeState(GameState::GAME_OVER);
        } else {
            player.reset(Coordinate(Coordinate::PLAYABLE_START_ROW, 1));
        }
    }
    
    void checkLevelProgression() {
        if (levelManager.isLevelComplete(enemies, score)) {
            stateManager.changeState(GameState::LEVEL_COMPLETE);
            score += levelManager.calculateTimeBonus(levelTimer, 
                                                   levelManager.getCurrentLevel());
        }
    }
    
    void spawnPowerUps() {
        if (levelManager.shouldSpawnPowerUp(levelTimer)) {
            powerUps.push_back(levelManager.createRandomPowerUp());
            levelManager.updatePowerUpSpawnTime(levelTimer);
        }
    }
    
    void handleMenuState() {
        InputAction action = inputManager.getMenuInput();
        switch (action) {
            case InputAction::CONFIRM:
                stateManager.changeState(GameState::PLAYING);
                initializeLevel();
                break;
            case InputAction::EXIT:
                window.Close();
                break;
            default:
                break;
        }
    }
    
    void handlePauseState() {
        InputAction action = inputManager.getPauseInput();
        switch (action) {
            case InputAction::CONFIRM:
                stateManager.changeState(GameState::PLAYING);
                break;
            case InputAction::EXIT:
                window.Close();
                break;
            default:
                break;
        }
    }
    
    void handleEndGameState() {
        InputAction action = inputManager.getEndGameInput();
        switch (action) {
            case InputAction::RESTART:
                restartGame();
                break;
            case InputAction::CONFIRM:
                if (stateManager.getCurrentState() == GameState::LEVEL_COMPLETE) {
                    nextLevel();
                }
                break;
            case InputAction::EXIT:
                window.Close();
                break;
            default:
                break;
        }
    }
    
    void initializeNewGame() {
        levelManager.reset();
        powerUpManager.reset();
        score = 0;
        playerLives = STARTING_LIVES;
        enemiesDefeated = 0;
        levelTimer = 0.0f;
        lastHarpoonTime = 0.0f;
        particles.clear();
        initializeLevel();
    }
    
    void initializeLevel() {
        levelManager.initializeLevel(levelManager.getCurrentLevel(), terrain, 
                                   player, enemies, powerUps, rocks);
        levelTimer = 0.0f;
        harpoons.clear();
        fireProjectiles.clear();
        particles.clear();
        
        if (!powerUpManager.hasPowerUpEffect(PowerUpType::SPEED_BOOST)) {
            player.setSpeedMultiplier(1.0f);
        }
    }
    
    void nextLevel() {
        levelManager.nextLevel();
        powerUpManager.reset();
        player.setSpeedMultiplier(1.0f);
        
        if (levelManager.getCurrentLevel() > MAX_LEVELS) {
            stateManager.changeState(GameState::VICTORY);
        } else {
            stateManager.changeState(GameState::PLAYING);
            initializeLevel();
        }
    }
    
    void restartGame() {
        stateManager.changeState(GameState::PLAYING);
        initializeNewGame();
    }
    
    void render() {
        BeginDrawing();
        ClearBackground(BLACK);
        
        screenShake.apply();
        
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
                uiManager.drawGameOverScreen(score, levelManager.getCurrentLevel());
                break;
            case GameState::LEVEL_COMPLETE:
                drawGameScene();
                uiManager.drawLevelCompleteScreen(score, levelTimer);
                break;
            case GameState::VICTORY:
                uiManager.drawVictoryScreen(score);
                break;
        }
        
        screenShake.reset();
        EndDrawing();
    }
    
    void drawGameScene() {
        float harpoonProgress = canFireHarpoon() ? 1.0f : 
                               (GetTime() - lastHarpoonTime) / 
                               powerUpManager.getHarpoonCooldown();
        
        uiManager.drawHUD(levelManager.getCurrentLevel(), score, 
                         levelManager.getTargetScore(), playerLives, 
                         levelTimer, powerUpManager.getActivePowerUps(),
                         canFireHarpoon(), harpoonProgress);
        
        renderer.drawTerrain(terrain);
        renderer.drawEnemies(enemies);
        renderer.drawHarpoons(harpoons, powerUpManager.getHasPowerShot());
        renderer.drawFireProjectiles(fireProjectiles);
        renderer.drawPowerUps(powerUps);
        renderer.drawPlayer(player, 
                          powerUpManager.hasPowerUpEffect(PowerUpType::SPEED_BOOST),
                          player.getIsDigging());
        renderer.drawRocks(rocks, player);
        
        particles.draw();
        
        uiManager.drawPowerUpNotification(powerUpManager.getPowerUpMessage(), 
                                        powerUpManager.getTimeSinceLastCollection());
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
