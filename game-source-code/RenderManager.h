#ifndef RENDERMANAGER_H
#define RENDERMANAGER_H

#include <raylib-cpp.hpp>
#include "Coordinate.h"
#include "Player.h"
#include "Enemy.h"
#include "Harpoon.h"
#include "PowerUp.h"
#include "Rock.h"
#include "BlockGrid.h"
#include "FireProjectile.h"
#include <string>

/**
 * @file RenderManager.h
 * @brief Centralized rendering system for all game entities
 */

/**
 * @class RenderManager
 * @brief Handles all visual rendering separated from game logic
 * 
 * RenderManager implements separation of presentation and logic:
 * - Converts grid coordinates to screen pixels
 * - Renders terrain, entities, and effects
 * - Applies visual enhancements (animations, particles)
 * - Maintains consistent cell-based rendering
 * 
 * Rendering pipeline:
 * 1. Terrain (earth blocks, tunnels, sky)
 * 2. Entities (enemies, rocks, power-ups)
 * 3. Player
 * 4. Weapons (harpoons, fire projectiles)
 * 5. Particles and effects
 * 
 * Visual features:
 * - Smooth animations using easing functions
 * - Pulsing effects for active states
 * - Color transitions for damage/power-ups
 * - Destruction animations
 * - Phase-through transparency
 * 
 * Coordinate system:
 * - Grid: 20 rows × 30 columns
 * - Cell size: 40×40 pixels
 * - Screen: 1200×800 pixels
 * - HUD area: Top 3 rows (120 pixels)
 * 
 * @note RenderManager has no game logic - pure presentation
 */
class RenderManager {
private:
    const int cellSize;
    const int screenWidth;
    const int screenHeight;
    const int hudHeight;

public:
    /**
     * @brief Construct render manager with screen dimensions
     * @param cellSz Size of each grid cell in pixels
     * @param screenW Screen width in pixels
     * @param screenH Screen height in pixels
     */
    RenderManager(int cellSz, int screenW, int screenH);
    
    /**
     * @brief Draw terrain grid (earth blocks and tunnels)
     * @param terrain BlockGrid to render
     */
    void drawTerrain(const BlockGrid& terrain);
    
    /**
     * @brief Draw player character
     * @param player Player to render
     * @param hasSpeedBoost true if speed boost active (glow effect)
     * @param isDigging true if digging (pulse effect)
     */
    void drawPlayer(const Player& player, bool hasSpeedBoost, bool isDigging);
    
    /**
     * @brief Draw all enemies
     * @param enemies Enemy vector to render
     */
    void drawEnemies(const std::vector<Enemy>& enemies);
    
    /**
     * @brief Draw all harpoons
     * @param harpoons Harpoon vector to render
     * @param hasPowerShot true for enhanced visual effect
     */
    void drawHarpoons(const std::vector<Harpoon>& harpoons, bool hasPowerShot);
    
    /**
     * @brief Draw all power-ups
     * @param powerUps PowerUp vector to render
     */
    void drawPowerUps(const std::vector<PowerUp>& powerUps);
    
    /**
     * @brief Draw all rocks
     * @param rocks Rock vector to render
     * @param player Player reference for danger indication
     */
    void drawRocks(const std::vector<Rock>& rocks, const Player& player);
    
    /**
     * @brief Draw fire projectiles
     * @param fires FireProjectile vector to render
     */
    void drawFireProjectiles(const std::vector<FireProjectile>& fires);
    
    /**
     * @brief Draw sky cell with animated clouds
     * @param x Screen X coordinate
     * @param y Screen Y coordinate
     */
    void drawSkyCell(int x, int y);

private:
    void drawEarthBlock(int x, int y);
    void drawTunnelCell(int x, int y);
    void drawDestroyedEnemy(int x, int y, float progress);
    void drawActiveEnemy(int x, int y, const Enemy& enemy);
    Color getPowerUpColor(PowerUpType type);
    std::string getPowerUpSymbol(PowerUpType type);
};

#endif // RENDERMANAGER_H
