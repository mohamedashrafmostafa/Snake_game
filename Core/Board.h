#pragma once
#include <vector>
#include <unordered_set>
#include <string>
#include "Position.h"
#include "Snake.h"
#include "Food.h"

class Board {
private:
    int width, height;
    std::vector<std::vector<char>> grid;
    std::unordered_set<Position, PositionHash> obstacles;

public:
    Board(int w, int h);
    void clear();
    bool isObstacle(Position pos) const;
    void addObstacle(Position pos);
    bool isInsideBoard(Position pos) const;

    const std::unordered_set<Position, PositionHash>& getObstacles() const { return obstacles; }
    int getWidth()  const { return width; }
    int getHeight() const { return height; }
};
