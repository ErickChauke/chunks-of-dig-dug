#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H

#include <raylib-cpp.hpp>
#include "EnemyLogic.h"

/**
 * @file InputManager.h
 * @brief Centralized input handling and action mapping
 */

/**
 * @enum InputAction
 * @brief High-level input actions mapped from raw keys
 */
enum class InputAction {
    NONE,         ///< No input
    MOVE_UP,      ///< Move player up
    MOVE_DOWN,    ///< Move player down
    MOVE_LEFT,    ///< Move player left
    MOVE_RIGHT,   ///< Move player right
    FIRE_HARPOON, ///< Fire weapon
    PAUSE,        ///< Pause game
    RESTART,      ///< Restart game
    CONFIRM,      ///< Confirm selection
    EXIT          ///< Exit to menu
};

/**
 * @class InputManager
 * @brief Centralizes input detection and action mapping
 * 
 * InputManager abstracts raw keyboard input into game actions:
 * - Separates input detection from game logic
 * - Provides context-specific input handling
 * - Maps multiple keys to same action
 * - Distinguishes pressed vs held states
 * 
 * Key mappings:
 * - Arrow keys: Movement (4-directional)
 * - SPACE: Fire harpoon
 * - P: Pause/unpause
 * - R: Restart game
 * - ENTER: Confirm/start
 * - ESC: Exit/back to menu
 * 
 * Input contexts:
 * - Menu: ENTER (start), ESC (exit)
 * - Gameplay: Movement, harpoon, pause
 * - Paused: P (resume), ESC (exit)
 * - End screens: R (restart), ENTER (continue), ESC (menu)
 * 
 * Design benefits:
 * - Easy to remap keys (change once here)
 * - Game logic doesn't reference specific keys
 * - Testable without actual keyboard
 * - Could support gamepad in future
 * 
 * @note Uses raylib's IsKeyPressed() and IsKeyDown()
 */
class InputManager {
public:
    InputManager() = default;
    
    /**
     * @brief Get input action for menu context
     * @return InputAction Mapped action (CONFIRM, EXIT, or NONE)
     */
    InputAction getMenuInput();
    
    /**
     * @brief Get input action for pause context
     * @return InputAction Mapped action (CONFIRM, EXIT, or NONE)
     */
    InputAction getPauseInput();
    
    /**
     * @brief Get input action for end game context
     * @return InputAction Mapped action (RESTART, CONFIRM, EXIT, or NONE)
     */
    InputAction getEndGameInput();
    
    /**
     * @brief Get movement direction from input
     * @return Direction Current directional input (or NONE)
     * @note Uses IsKeyDown() for continuous movement
     */
    Direction getMovementInput();
    
    /**
     * @brief Check if harpoon fire pressed
     * @return true if SPACE pressed this frame
     */
    bool isHarpoonPressed();
    
    /**
     * @brief Check if pause pressed
     * @return true if P pressed this frame
     */
    bool isPausePressed();
    
    /**
     * @brief Check if restart pressed
     * @return true if R pressed this frame
     */
    bool isRestartPressed();
    
    /**
     * @brief Check if exit pressed
     * @return true if ESC pressed this frame
     */
    bool isExitPressed();
    
private:
    bool wasKeyPressed(int key);
    bool isKeyCurrentlyDown(int key);
};

#endif // INPUTMANAGER_H
