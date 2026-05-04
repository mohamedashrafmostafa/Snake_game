#pragma once
#include "Position.h"

// Store moves to use Stack for the Undo feature
struct Move {
    Position previousTail; // Previous tail position to restore it
    bool ateFood;          // Whether food was eaten in this move
};