#ifndef GAMESTATE_H
#define GAMESTATE_H

#include <string>

enum class GameState {
    MENU,
    PLAYING,
    PAUSED,
    GAME_OVER,
    LEVEL_COMPLETE,
    VICTORY,
    LOADING,
    SETTINGS
};

class GameStateManager {
private:
    GameState currentState;
    GameState previousState;
    float stateTimer;
    bool canTransition;

public:
    GameStateManager() : currentState(GameState::MENU), previousState(GameState::MENU), 
                        stateTimer(0.0f), canTransition(true) {}
    
    bool changeState(GameState newState);
    GameState getCurrentState() const { return currentState; }
    GameState getPreviousState() const { return previousState; }
    void update(float deltaTime);
    float getStateTime() const { return stateTimer; }
    bool canMakeTransition() const { return canTransition; }
    
    static std::string getStateName(GameState state);
    static bool isValidTransition(GameState from, GameState to);
};

#endif // GAMESTATE_H
