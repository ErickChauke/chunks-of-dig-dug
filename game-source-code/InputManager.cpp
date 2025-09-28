#include "InputManager.h"

InputAction InputManager::getMenuInput() {
    if (wasKeyPressed(KEY_ENTER)) return InputAction::CONFIRM;
    if (wasKeyPressed(KEY_ESCAPE)) return InputAction::EXIT;
    return InputAction::NONE;
}

InputAction InputManager::getPauseInput() {
    if (wasKeyPressed(KEY_P)) return InputAction::CONFIRM;
    if (wasKeyPressed(KEY_ESCAPE)) return InputAction::EXIT;
    return InputAction::NONE;
}

InputAction InputManager::getEndGameInput() {
    if (wasKeyPressed(KEY_R)) return InputAction::RESTART;
    if (wasKeyPressed(KEY_ENTER)) return InputAction::CONFIRM;
    if (wasKeyPressed(KEY_ESCAPE)) return InputAction::EXIT;
    return InputAction::NONE;
}

Direction InputManager::getMovementInput() {
    if (isKeyCurrentlyDown(KEY_UP)) return Direction::UP;
    if (isKeyCurrentlyDown(KEY_DOWN)) return Direction::DOWN;
    if (isKeyCurrentlyDown(KEY_LEFT)) return Direction::LEFT;
    if (isKeyCurrentlyDown(KEY_RIGHT)) return Direction::RIGHT;
    return Direction::NONE;
}

bool InputManager::isHarpoonPressed() {
    return wasKeyPressed(KEY_SPACE);
}

bool InputManager::isPausePressed() {
    return wasKeyPressed(KEY_P);
}

bool InputManager::isRestartPressed() {
    return wasKeyPressed(KEY_R);
}

bool InputManager::isExitPressed() {
    return wasKeyPressed(KEY_ESCAPE);
}

bool InputManager::wasKeyPressed(int key) {
    return IsKeyPressed(key);
}

bool InputManager::isKeyCurrentlyDown(int key) {
    return IsKeyDown(key);
}
