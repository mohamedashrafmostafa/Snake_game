#ifndef PROJECT_SNAKE_H
#define PROJECT_SNAKE_H
#include <deque>
#include <unordered_set>
/* about unordered set
  to make sure that there is no 2 part of snake in the same position
unordered set is a kind of container that contains all the position the snake body occupies
when adding a new block or node to the body of the snake:
we compare the new node positions with the positions in the set , if it exists,then that means there is 2 blocks in the same position
so--> the snake touches itself --> game over
or,
we just add the new node if newPosition!= any block position
*/
#include "Position.h"
using namespace std;
class Snake {
private:
    Position direction;
    deque<Position> body;
    unordered_set<Position,PositionHash>pos;
public:
    Snake(Position startPos):direction(startPos) {}//start of the game
    void move(); //add head,remove tail
    void grow();//add head , keep tail
    void setDirection(Position newDir);//giving instruction to the next direction should be moved to
    void collide(); // keep head, remove tail
    bool selfCollide(Position nextPos);//body touches itself-->GAME OVER
    bool wallTouch();//Game OVER too
    Position getHead();//return head position
    deque<Position>getBody();//return all the body position as deque
    Position getDirection(); //get direction of specific block position
};
#endif

GAME_SNAKE_H