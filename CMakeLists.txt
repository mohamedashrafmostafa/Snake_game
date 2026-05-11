cmake_minimum_required(VERSION 3.20)
project(Snake_game)

set(CMAKE_CXX_STANDARD 20)

# ─── QT CONFIGURATION ─────────────────────────────────────────────
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 COMPONENTS Widgets REQUIRED)

# ─── EXECUTABLE CONFIGURATION ─────────────────────────────────────
add_executable(SnakePlusPlus
        main.cpp
        Core/Board.cpp
        Core/Board.h
        Core/Food.cpp
        Core/Food.h
        Core/Game.cpp
        Core/Game.h
        Core/InputHandler.cpp
        Core/InputHandler.h
        Core/Leaderboard.cpp
        Core/Leaderboard.h
        Core/Move.h
        Core/Position.h


        Core/snake.cpp
        Core/snake.h
        GUI/Mainwindow.cpp
        GUI/Mainwindow.h
        GUI/BoardWidget.cpp
        GUI/BoardWidget.h
)

target_link_libraries(SnakePlusPlus PRIVATE Qt6::Widgets)