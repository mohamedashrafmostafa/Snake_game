#pragma once
#include "Position.h"

// Stores one game tick's state so the Stack can undo it
struct Move {
    Position previousTail;       // Tail position before this move (to restore on undo)
    Position previousDirection;  // Direction before this move (to restore on undo)
    bool ateFood;                // Whether food was eaten in this move
    int earnedPoints;            // ADDED: exact points earned this tick (fixes undo score bug)
};