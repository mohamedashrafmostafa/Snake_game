cmake_minimum_required(VERSION 4.1)
project(PRJ)

set(CMAKE_CXX_STANDARD 20)

add_executable(PRJ main.cpp
        Snake.h
        Postion.h
        Game.cpp
        Snake.cpp
        Game.h
        Board.cpp
        Board.h
        Food.cpp
        Food.h
        Leaderboard.cpp
        Leaderboard.h
        PowerUp.h
        PowerUp.cpp
        InputHandler.h
        InputHandler.cpp
        Utils.cpp
        Utils.h
        Move.h)
