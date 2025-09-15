#include <raylib-cpp.hpp>
#include <iostream>
#include "Coordinate.h"
#include "BlockGrid.h"
#include "Player.h"

const int SCREEN_WIDTH = 1200;
const int SCREEN_HEIGHT = 800;
const int CELL_SIZE = 40;

class DigDugGame {
private:
    raylib::Window window;
    BlockGrid terrain;
    Player player;
    bool gameRunning;

public:
    DigDugGame() : window(SCREEN_WIDTH, SCREEN_HEIGHT, "Underground Adventure"),
                   player(Coordinate(1, 1)), gameRunning(true) {
        window.SetTargetFPS(60);
        terrain.initializeDefaultMap();
    }
    
    void run() {
        while (!window.ShouldClose() && gameRunning) {
            update();
            render();
        }
    }
    
private:
    void update() {
        if (IsKeyPressed(KEY_ESCAPE)) {
            gameRunning = false;
        }
        
        if (IsKeyPressed(KEY_R)) {
            player.reset(Coordinate(1, 1));
            terrain.initializeDefaultMap();
        }
    }
    
    void render() {
        BeginDrawing();
        ClearBackground(BLACK);
        
        for (int row = 0; row < Coordinate::WORLD_ROWS; ++row) {
            for (int col = 0; col < Coordinate::WORLD_COLS; ++col) {
                Coordinate pos(row, col);
                int screenX = col * CELL_SIZE;
                int screenY = row * CELL_SIZE;
                
                if (terrain.isLocationBlocked(pos)) {
                    DrawRectangle(screenX, screenY, CELL_SIZE, CELL_SIZE, BROWN);
                    DrawRectangleLines(screenX, screenY, CELL_SIZE, CELL_SIZE, DARKBROWN);
                } else {
                    DrawRectangle(screenX, screenY, CELL_SIZE, CELL_SIZE, DARKGRAY);
                    DrawRectangleLines(screenX, screenY, CELL_SIZE, CELL_SIZE, GRAY);
                }
            }
        }
        
        Coordinate pos = player.getPosition();
        int screenX = pos.col * CELL_SIZE;
        int screenY = pos.row * CELL_SIZE;
        int centerX = screenX + CELL_SIZE / 2;
        int centerY = screenY + CELL_SIZE / 2;
        int playerSize = CELL_SIZE - 8;
        
        DrawCircle(centerX, centerY, playerSize / 2, YELLOW);
        DrawCircleLines(centerX, centerY, playerSize / 2, ORANGE);
        
        DrawText("Underground Adventure", 10, 10, 24, WHITE);
        std::string posText = "Position: (" + std::to_string(pos.row) + 
                             ", " + std::to_string(pos.col) + ")";
        DrawText(posText.c_str(), 10, SCREEN_HEIGHT - 80, 14, WHITE);
        
        std::string tunnelText = "Tunnels: " + std::to_string(player.getTunnelsCreated());
        DrawText(tunnelText.c_str(), 10, SCREEN_HEIGHT - 60, 14, GREEN);
        
        DrawText("R: Reset | ESC: Exit", 10, SCREEN_HEIGHT - 40, 14, YELLOW);
        EndDrawing();
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
