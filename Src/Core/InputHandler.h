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
    // This function must be non-blocking.
    // It checks if a key was pressed and returns the corresponding InputKey.
    static InputKey getKeyPress();
};