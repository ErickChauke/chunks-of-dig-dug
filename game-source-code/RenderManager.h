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
    void drawSkyCell(int x, int y);
};

#endif // RENDERMANAGER_H
