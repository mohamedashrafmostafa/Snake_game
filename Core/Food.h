#pragma once
#include "Position.h"

enum FoodType { NORMAL, BONUS, SPEED_UP };

class Food {
private:
    Position pos;
    FoodType type;
    int points;

public:
    Food(Position startPos = {0, 0}, FoodType t = NORMAL);
    
    Position getPosition() const;
    FoodType getType() const;
    int getPoints() const;
    
    void setPosition(Position newPos);
    void setType(FoodType newType);
};