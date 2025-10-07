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

class RenderManager {
private:
    const int cellSize;
    const int screenWidth;
    const int screenHeight;
    const int hudHeight;

public:
    RenderManager(int cellSz, int screenW, int screenH);
    
    void drawTerrain(const BlockGrid& terrain);
    void drawPlayer(const Player& player, bool hasSpeedBoost, bool isDigging);
    void drawEnemies(const std::vector<Enemy>& enemies);
    void drawHarpoons(const std::vector<Harpoon>& harpoons, bool hasPowerShot);
    void drawPowerUps(const std::vector<PowerUp>& powerUps);
    void drawRocks(const std::vector<Rock>& rocks, const Player& player);
    void drawFireProjectiles(const std::vector<FireProjectile>& fires);
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
