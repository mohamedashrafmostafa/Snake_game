#pragma once
#include "Position.h"

enum PowerUpType { SPEED_UP, INVINCIBILITY, SCORE_MULTIPLIER };

struct PowerUp {
    Position pos;
    PowerUpType type;
    int expirationTime; // Time when the power-up expires

    // Comparison operator for Priority Queue to sort by closest expiration
    bool operator<(const PowerUp& other) const {
        return expirationTime > other.expirationTime; 
    }
};