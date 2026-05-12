#include "Food.h"
 
Food::Food(Position startPos, FoodType t) : pos(startPos), type(t) {
    switch (t) {
        case NORMAL:   points = 10; break;
        case BONUS:    points = 25; break;
        case SPEED_UP: points = 15; break;
        default:       points = 10; break;
    }
}
 
Position Food::getPosition() const {
    return pos;
}
 
FoodType Food::getType() const {
    return type;
}
 
int Food::getPoints() const {
    return points;
}
 
void Food::setPosition(Position newPos) {
    pos = newPos;
}
 
void Food::setType(FoodType newType) {
    type = newType;
    switch (newType) {
        case NORMAL:   points = 10; break;
        case BONUS:    points = 25; break;
        case SPEED_UP: points = 15; break;
        default:       points = 10; break;
    }
}