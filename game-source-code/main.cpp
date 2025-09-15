#include <raylib-cpp.hpp>
#include <iostream>
#include "Coordinate.h"
#include "BlockGrid.h"
#include "Player.h"

const int SCREEN_WIDTH = 1200;
const int SCREEN_HEIGHT = 800;

class DigDugGame {
private:
    raylib::Window window;
    bool gameRunning;

public:
    DigDugGame() : window(SCREEN_WIDTH, SCREEN_HEIGHT, "Underground Adventure"),
                   gameRunning(true) {
        window.SetTargetFPS(60);
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
