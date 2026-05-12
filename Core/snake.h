#pragma once
#include <deque>
#include <unordered_set>
#include "Position.h"

class Snake {
private:
    std::deque<Position> body;
    Position direction;
    std::unordered_set<Position, PositionHash> bodySet;

public:
    Snake();
    void move();
    void grow();
    bool checkSelfCollision() const;
    Position getHead() const;
    std::deque<Position> getBody() const;
    
    void setDirection(Position dir);
    Position getDirection() const;

    void setHead(Position newHead);
};