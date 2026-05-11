#pragma once
#include "Position.h"

// Types of food available in the game
enum FoodType { NORMAL, BONUS, SPEED_UP };

class Food {
private:
    Position pos;
    FoodType type;
    int points; // The score value of this food

public:
    // Constructor with a default type of NORMAL
    Food(Position startPos = {0, 0}, FoodType t = NORMAL);
    
    // Getters
    Position getPosition() const;
    FoodType getType() const;
    int getPoints() const;
    
    // Setters
    void setPosition(Position newPos);
    void setType(FoodType newType);
};