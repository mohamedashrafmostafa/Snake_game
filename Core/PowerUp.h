#pragma once
#include "Position.h"

enum PowerUpType { P_SPEED_UP, INVINCIBILITY, SCORE_MULTIPLIER };

struct PowerUp {
    Position pos;
    PowerUpType type;
    int expirationTime;

    bool operator<(const PowerUp& other) const {
        return expirationTime > other.expirationTime; 
    }
};