#ifndef GAMESTATE_H
#define GAMESTATE_H

#include <string>

/**
 * @file GameState.h
 * @brief Game state machine for menu/gameplay flow control
 */

/**
 * @enum GameState
 * @brief All possible game states in the state machine
 */
enum class GameState {
    MENU,           ///< Main menu (press ENTER to start)
    PLAYING,        ///< Active gameplay
    PAUSED,         ///< Game paused (press P to resume)
    GAME_OVER,      ///< Player died with no lives remaining
    LEVEL_COMPLETE, ///< Level objectives achieved
    VICTORY,        ///< All levels completed
    LOADING,        ///< Loading next level
    SETTINGS        ///< Settings menu (unused)
};

/**
 * @class GameStateManager
 * @brief Finite state machine for game flow control
 * 
 * GameStateManager implements a validated state machine that:
 * - Tracks current and previous game states
 * - Validates state transitions before applying
 * - Prevents invalid transitions (e.g., PLAYING → SETTINGS)
 * - Tracks time spent in each state
 * - Enforces minimum transition cooldown (0.1s)
 * 
 * Valid transitions:
 * - MENU → PLAYING, SETTINGS
 * - PLAYING → PAUSED, GAME_OVER, LEVEL_COMPLETE, MENU
 * - PAUSED → PLAYING, MENU
 * - GAME_OVER → MENU, PLAYING (restart)
 * - LEVEL_COMPLETE → PLAYING (next level), VICTORY, MENU
 * - VICTORY → MENU, PLAYING (restart)
 * - LOADING → PLAYING
 * - SETTINGS → MENU
 * 
 * Transition cooldown prevents rapid state changes that
 * could cause input processing issues or visual glitches.
 * 
 * @note Uses static validation methods for testability
 */
class GameStateManager {
private:
    GameState currentState;
    GameState previousState;
    float stateTimer;
    bool canTransition;

public:
    GameStateManager() : currentState(GameState::MENU), previousState(GameState::MENU), 
                        stateTimer(0.0f), canTransition(true) {}
    
    /**
     * @brief Attempt to change game state
     * @param newState Desired target state
     * @return true if transition successful
     * @note Validates transition and enforces cooldown
     */
    bool changeState(GameState newState);
    
    GameState getCurrentState() const { return currentState; }
    GameState getPreviousState() const { return previousState; }
    
    /**
     * @brief Update state timer and cooldown
     * @param deltaTime Frame time in seconds
     */
    void update(float deltaTime);
    
    float getStateTime() const { return stateTimer; }
    bool canMakeTransition() const { return canTransition; }
    
    /**
     * @brief Get human-readable state name
     * @param state State to convert
     * @return std::string State name (e.g., "PLAYING")
     */
    static std::string getStateName(GameState state);
    
    /**
     * @brief Validate state transition
     * @param from Source state
     * @param to Target state
     * @return true if transition allowed
     */
    static bool isValidTransition(GameState from, GameState to);
};

#endif // GAMESTATE_H
