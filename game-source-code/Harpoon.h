#ifndef HARPOON_H
#define HARPOON_H

#include "GameObject.h"
#include "Coordinate.h"
#include "EnemyLogic.h"
#include <vector>

enum class HarpoonState {
    EXTENDING,
    RETRACTING,
    IDLE
};

// Forward declaration
class Player;

class Harpoon : public GameObject {
private:
    Direction direction;
    float speed;
    float maxRange;
    float currentLength;
    HarpoonState state;
    Coordinate startPosition;
    Player* playerRef; // Reference to player object
    std::vector<Coordinate> segments;

public:
    Harpoon(Coordinate startPos, Direction dir, Player* player);
    
    void update() override;
    void render() override;
    
    bool checkEnemyHit(Coordinate enemyPos);
    Direction getDirection() const;
    HarpoonState getState() const;
    const std::vector<Coordinate>& getSegments() const;
    
    void updatePlayerConnection();

private:
    void extend();
    void retract();
    void recalculateSegments();
};

#endif // HARPOON_H
