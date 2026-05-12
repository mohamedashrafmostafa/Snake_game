#include "snake.h"
Snake::Snake () {
    direction={1,0};
    body.push_front(direction);  
}
int bSize;
void Snake::move(){
Position currhead=body.front();
Position newHead;
newHead.x=direction.x+currhead.x;
newHead.y=direction.y+currhead.y;
body.push_front(newHead);
bodySet.insert(newHead);
Position oldTail= body.back();
bodySet.erase(oldTail);
body.pop_back();
}
void Snake:: grow(){
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
if (bodySet.count(newHead)){
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

void Snake::setDirection(Position dir) {
    direction = dir;
}

Position Snake::getDirection() const {
    return direction;
}

void Snake::setHead(Position newHead) {
    if (!body.empty()) {
        Position oldHead = body.front();
        bodySet.erase(oldHead);
        body.front() = newHead;
        bodySet.insert(newHead);
    }
}