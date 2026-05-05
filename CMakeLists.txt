cmake_minimum_required(VERSION 4.1)
project(Snake_game)

set(CMAKE_CXX_STANDARD 14)

include_directories(Core)

add_executable(Snake_game
        Core/snake.cpp
        Core/snake.h
        GUI/Mainwindow.cpp
        GUI/Mainwindow.h)
