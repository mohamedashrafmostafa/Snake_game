#pragma once
#include <vector>
#include <unordered_set>
#include <string>
#include "Position.h"
#include "Snake.h"
#include "Food.h"         // ADDED: needed for the fixed render() signature

class Board {
private:
    int width, height;
    std::vector<std::vector<char>> grid;                    // 2D grid representing the board
    std::unordered_set<Position, PositionHash> obstacles;   // Obstacles for O(1) fast lookup

public:
    Board(int w, int h);
    void clear();
    void render(const Snake& snake, const Food& food);  // FIXED: was "const Position& food"
    bool isObstacle(Position pos) const;                // O(1) check for obstacles
    void addObstacle(Position pos);
    bool isInsideBoard(Position pos) const;             // Check board boundaries
    void loadMapFromFile(std::string filename);         // Extra feature: load different maps

    // ── Getters for GUI rendering ─────────────────────
    const std::unordered_set<Position, PositionHash>& getObstacles() const { return obstacles; }
    int getWidth()  const { return width; }
    int getHeight() const { return height; }
};
