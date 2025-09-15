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
    bool gameRunning;

public:
    DigDugGame() : window(SCREEN_WIDTH, SCREEN_HEIGHT, "Underground Adventure"),
                   gameRunning(true) {
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
        
        DrawText("Underground Adventure", 10, 10, 24, WHITE);
        DrawText("ESC: Exit", 10, SCREEN_HEIGHT - 40, 14, YELLOW);
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
