cmake_minimum_required(VERSION 4.1)
project(PRJ)

set(CMAKE_CXX_STANDARD 20)

add_executable(PRJ
        Core/main.cpp
        Core/Snake.h
        Core/Snake.cpp
        Core/Position.h
        Core/Game.h
        Core/Game.cpp
        Core/Board.h
        Core/Board.cpp
        Core/Food.h
        Core/Food.cpp
        Core/Leaderboard.h
        Core/Leaderboard.cpp
        Core/PowerUp.h
        Core/PowerUp.cpp
        Core/InputHandler.h
        Core/InputHandler.cpp
        Core/Utils.h
        Core/Utils.cpp
        Core/Move.h

)
