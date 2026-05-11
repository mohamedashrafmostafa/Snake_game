#pragma once

// Enum to represent the possible inputs from the player
enum class InputKey {
    UP,
    DOWN,
    LEFT,
    RIGHT,//hh
    PAUSE,
    QUIT,
    UNDO,       // For the undo feature
    NONE        // When no key is pressed
};

class InputHandler {
public:
    // Note: GUI input is handled directly by Qt events, not through this class
    // This is kept for compatibility with InputKey enum
};