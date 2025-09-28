#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H

#include <raylib-cpp.hpp>
#include "EnemyLogic.h"

enum class InputAction {
    NONE,
    MOVE_UP,
    MOVE_DOWN,
    MOVE_LEFT,
    MOVE_RIGHT,
    FIRE_HARPOON,
    PAUSE,
    RESTART,
    CONFIRM,
    EXIT
};

class InputManager {
public:
    InputManager() = default;
    
    InputAction getMenuInput();
    InputAction getPauseInput();
    InputAction getEndGameInput();
    
    Direction getMovementInput();
    bool isHarpoonPressed();
    bool isPausePressed();
    bool isRestartPressed();
    bool isExitPressed();
    
private:
    bool wasKeyPressed(int key);
    bool isKeyCurrentlyDown(int key);
};

#endif // INPUTMANAGER_H
