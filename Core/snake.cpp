#include <iostream>
#include "snake.h"
Snake::Snake () {
    direction={1,0};//4 values: left,right,up,down
    body.push_front(direction);  
}
int bSize;
void Snake::move(){
Position currhead=body.front();
Position newHead;
newHead.x=direction.x+currhead.x;
newHead.y=direction.y+currhead.y;
body.push_front(newHead);//add to queue
bodySet.insert(newHead);// add to unordered set(to see if it's already there or not LATER:) )
Position oldTail= body.back();
bodySet.erase(oldTail);//remove rear position from the set
body.pop_back();//remove Position from deque
}
void Snake:: grow(){
//if the snake eats
Position currhead=body.front();
Position newHead;
newHead.x=direction.x+currhead.x;
newHead.y=direction.y+currhead.y;
body.push_front(newHead);
bodySet.insert(newHead);
}

bool Snake::checkSelfCollision() const{
    Position currhead=body.front();
Position newHead;
newHead.x=direction.x+currhead.x;
newHead.y=direction.y+currhead.y;
if (bodySet.count(newHead)){// count:unordered set's method checks if it contains specific value
    // The deque will have 2 same positions into it -> Game ends
    return true;
}
else{
    return false;
}
}
Position Snake::getHead() const{
Position currhead=body.front();
return currhead;
}
 std::deque<Position> Snake::getBody() const {
    return body;
}