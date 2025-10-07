#ifndef POWERUPEFFECT_H
#define POWERUPEFFECT_H

#include "PowerUp.h"
#include <raylib-cpp.hpp>

struct PowerUpEffect {
    PowerUpType type;
    float startTime;
    float duration;
    bool active;
    
    PowerUpEffect(PowerUpType t, float d) : type(t), startTime(GetTime()), 
                                           duration(d), active(true) {}
    
    bool isExpired() const {
        return (GetTime() - startTime) >= duration;
    }
    
    float getTimeRemaining() const {
        return duration - (GetTime() - startTime);
    }
};

#endif // POWERUPEFFECT_H
