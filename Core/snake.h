#pragma once
#include <deque>
#include <unordered_set>
#include "Position.h"

class Snake {
private:
    std::deque<Position> body;      // Snake body (head is at the front)
    Position direction;             // Current movement direction
    std::unordered_set<Position, PositionHash> bodySet;  // For O(1) self-collision detection

public:
    Snake();
    void move();                    // Move the snake (add head, remove tail)
    void grow();                    // Increase length (add head, keep tail)
    bool checkSelfCollision() const; // Check self-collision in O(1)
    Position getHead() const;
    std::deque<Position> getBody() const;
    
    //void setDirection(Position dir);
    //Position getDirection() const;
};