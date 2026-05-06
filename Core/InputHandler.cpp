#include "InputHandler.h"
#ifdef _WIN32
#include <conio.h>
#else
#endif

// ────────────────────────────────────────────────────────
// Added by UI/GUI Integration to resolve Linker Errors
// The other group members declared this but left it empty.
// ────────────────────────────────────────────────────────

InputKey InputHandler::getKeyPress() {
#ifdef _WIN32
    if (_kbhit()) {
        int ch = _getch();
        if (ch == 0 || ch == 224) { // Arrow keys
            ch = _getch();
            switch (ch) {
                case 72: return InputKey::UP;
                case 80: return InputKey::DOWN;
                case 75: return InputKey::LEFT;
                case 77: return InputKey::RIGHT;
            }
        } else {
            switch (ch) {
                case 'w': case 'W': return InputKey::UP;
                case 's': case 'S': return InputKey::DOWN;
                case 'a': case 'A': return InputKey::LEFT;
                case 'd': case 'D': return InputKey::RIGHT;
                case 'p': case 'P': return InputKey::PAUSE;
                case 'q': case 'Q': return InputKey::QUIT;
                case 'z': case 'Z': return InputKey::UNDO;
            }
        }
    }
#endif
    return InputKey::NONE;
}