#ifndef PROJECT_POSITION_H
#define PROJECT_POSITION_H
#include <functional>
using Type=int;
struct Position {
    Type x;
    Type y;
    bool operator==(const Position &other) const {
        return (other.x==x && other.y==y);
    }
};
struct PositionHash {
    std::size_t operator()(const Position& pos) const {
        return std::hash<Type>()(pos.x) ^ (std::hash<Type>()(pos.y) << 1);
    }
};

#endif

