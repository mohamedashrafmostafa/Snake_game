cmake_minimum_required(VERSION 3.20)
project(Snake_game)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 20)

# ─── QT CONFIGURATION ─────────────────────────────────────────────
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Find the Qt6 Widgets module
find_package(Qt6 COMPONENTS Widgets REQUIRED)

# ─── EXECUTABLE CONFIGURATION ─────────────────────────────────────
# This MUST come before target_link_libraries!
add_executable(SnakePlusPlus
        # The Main
        GUI/main.cpp

        # The core files
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
        Core/Postion.h
        Core/PowerUp.cpp
        Core/PowerUp.h
        Core/snake.cpp
        Core/snake.h
        Core/Utils.cpp
        Core/Utils.h

        #GUI Files
        GUI/Mainwindow.cpp
        GUI/Mainwindow.h
        GUI/BoardWidget.cpp
        GUI/BoardWidget.h
        GUI/mainwindow.ui
        GUI/resources.qrc
        icon.rc
)


target_link_libraries(SnakePlusPlus PRIVATE Qt6::Widgets)