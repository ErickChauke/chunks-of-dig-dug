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
        handleInput();
        player.update();
    }
    
    void handleInput() {
        player.handleMovement(terrain);
        
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
        
        drawTerrain();
        drawPlayer();
        drawUI();
        
        EndDrawing();
    }
    
    void drawTerrain() {
        for (int row = 0; row < Coordinate::WORLD_ROWS; ++row) {
            for (int col = 0; col < Coordinate::WORLD_COLS; ++col) {
                Coordinate pos(row, col);
                int screenX = col * CELL_SIZE;
                int screenY = row * CELL_SIZE;
                
                if (terrain.isLocationBlocked(pos)) {
                    DrawRectangle(screenX, screenY, CELL_SIZE, CELL_SIZE, BROWN);
                    DrawRectangleLines(screenX, screenY, CELL_SIZE, CELL_SIZE, DARKBROWN);
                    
                    DrawCircle(screenX + CELL_SIZE/4, screenY + CELL_SIZE/4, 2, DARKBROWN);
                    DrawCircle(screenX + 3*CELL_SIZE/4, screenY + 3*CELL_SIZE/4, 1, DARKBROWN);
                } else {
                    DrawRectangle(screenX, screenY, CELL_SIZE, CELL_SIZE, DARKGRAY);
                    DrawRectangleLines(screenX, screenY, CELL_SIZE, CELL_SIZE, GRAY);
                    
                    DrawCircle(screenX + CELL_SIZE/2, screenY + CELL_SIZE/2, 1, LIGHTGRAY);
                }
            }
        }
        
        DrawRectangleLines(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, WHITE);
    }
    
    void drawPlayer() {
        Coordinate pos = player.getPosition();
        int screenX = pos.col * CELL_SIZE;
        int screenY = pos.row * CELL_SIZE;
        int centerX = screenX + CELL_SIZE / 2;
        int centerY = screenY + CELL_SIZE / 2;
        int playerSize = CELL_SIZE - 8;
        
        DrawCircle(centerX, centerY, playerSize / 2, YELLOW);
        DrawCircleLines(centerX, centerY, playerSize / 2, ORANGE);
        
        int eyeSize = 3;
        int eyeOffset = playerSize / 6;
        DrawCircle(centerX - eyeOffset, centerY - eyeOffset, eyeSize, BLACK);
        DrawCircle(centerX + eyeOffset, centerY - eyeOffset, eyeSize, BLACK);
        
        DrawCircleLines(centerX, centerY + eyeOffset/2, eyeOffset, BLACK);
        
        Direction lastDir = player.getLastMoveDirection();
        if (lastDir != Direction::NONE) {
            int indicatorX = centerX;
            int indicatorY = centerY;
            
            switch (lastDir) {
                case Direction::UP:    indicatorY -= playerSize / 3; break;
                case Direction::DOWN:  indicatorY += playerSize / 3; break;
                case Direction::LEFT:  indicatorX -= playerSize / 3; break;
                case Direction::RIGHT: indicatorX += playerSize / 3; break;
                case Direction::NONE:  break;
            }
            
            DrawCircle(indicatorX, indicatorY, 2, WHITE);
        }
        
        if (player.getIsDigging()) {
            float pulse = sin(GetTime() * 10.0f) * 0.4f + 0.6f;
            DrawCircleLines(centerX, centerY, playerSize / 2 + 6, ColorAlpha(ORANGE, pulse));
        }
    }
    
    void drawUI() {
        DrawText("Underground Adventure", 10, 10, 24, WHITE);
        
        Coordinate pos = player.getPosition();
        std::string posText = "Position: (" + std::to_string(pos.row) + 
                             ", " + std::to_string(pos.col) + ")";
        DrawText(posText.c_str(), 10, SCREEN_HEIGHT - 80, 14, WHITE);
        
        std::string tunnelText = "Tunnels: " + std::to_string(player.getTunnelsCreated());
        DrawText(tunnelText.c_str(), 10, SCREEN_HEIGHT - 60, 14, GREEN);
        
        DrawText("Arrow Keys: Move | R: Reset | ESC: Exit", 10, SCREEN_HEIGHT - 40, 14, YELLOW);
        
        if (player.getIsMoving()) {
            DrawText("Moving", SCREEN_WIDTH - 80, 10, 14, GREEN);
        }
        if (player.getIsDigging()) {
            DrawText("Digging", SCREEN_WIDTH - 80, 30, 14, ORANGE);
        }
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
