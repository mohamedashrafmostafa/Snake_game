#include "Board.h"

// ────────────────────────────────────────────────────────
// Added by UI/GUI Integration to resolve Linker Errors
// The other group members declared these in Board.h but
// left Board.cpp empty. We are implementing them so the GUI builds.
// ────────────────────────────────────────────────────────

Board::Board(int w, int h) : width(w), height(h) {
    grid.resize(h, std::vector<char>(w, ' '));
}

void Board::clear() {
    obstacles.clear();
}


bool Board::isObstacle(Position pos) const {
    // O(1) unordered_set lookup
    return obstacles.count(pos) > 0;
}

void Board::addObstacle(Position pos) {
    obstacles.insert(pos);
}

bool Board::isInsideBoard(Position pos) const {
    return pos.x >= 0 && pos.x < width && pos.y >= 0 && pos.y < height;
}
