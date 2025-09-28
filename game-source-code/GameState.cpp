#include "GameState.h"
#include <raylib-cpp.hpp>

bool GameStateManager::changeState(GameState newState) {
    if (!canTransition || !isValidTransition(currentState, newState)) {
        return false;
    }
    
    previousState = currentState;
    currentState = newState;
    stateTimer = 0.0f;
    
    return true;
}

void GameStateManager::update(float deltaTime) {
    stateTimer += deltaTime;
    canTransition = (stateTimer >= 0.1f);
}

std::string GameStateManager::getStateName(GameState state) {
    switch (state) {
        case GameState::MENU: return "MENU";
        case GameState::PLAYING: return "PLAYING";
        case GameState::PAUSED: return "PAUSED";
        case GameState::GAME_OVER: return "GAME_OVER";
        case GameState::LEVEL_COMPLETE: return "LEVEL_COMPLETE";
        case GameState::VICTORY: return "VICTORY";
        case GameState::LOADING: return "LOADING";
        case GameState::SETTINGS: return "SETTINGS";
    }
    return "UNKNOWN";
}

bool GameStateManager::isValidTransition(GameState from, GameState to) {
    switch (from) {
        case GameState::MENU:
            return (to == GameState::PLAYING || to == GameState::SETTINGS);
        case GameState::PLAYING:
            return (to == GameState::PAUSED || to == GameState::GAME_OVER || 
                   to == GameState::LEVEL_COMPLETE || to == GameState::MENU);
        case GameState::PAUSED:
            return (to == GameState::PLAYING || to == GameState::MENU);
        case GameState::GAME_OVER:
            return (to == GameState::MENU || to == GameState::PLAYING);
        case GameState::LEVEL_COMPLETE:
            return (to == GameState::PLAYING || to == GameState::VICTORY || to == GameState::MENU);
        case GameState::VICTORY:
            return (to == GameState::MENU || to == GameState::PLAYING);
        case GameState::LOADING:
            return (to == GameState::PLAYING);
        case GameState::SETTINGS:
            return (to == GameState::MENU);
    }
    return false;
}
