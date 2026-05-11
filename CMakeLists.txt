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
        Src/main.cpp

        # The core files
        Src/Core/Board.cpp
        Src/Core/Board.h
        Src/Core/Food.cpp
        Src/Core/Food.h
        Src/Core/Game.cpp
        Src/Core/Game.h
        Src/Core/InputHandler.cpp
        Src/Core/InputHandler.h
        Src/Core/Leaderboard.cpp
        Src/Core/Leaderboard.h
        Src/Core/Move.h
        Src/Core/Position.h
        Src/Core/PowerUp.h
        Src/Core/snake.cpp
        Src/Core/snake.h


        #GUI Files
        Src/GUI/Mainwindow.cpp
        Src/GUI/Mainwindow.h
        Src/GUI/BoardWidget.cpp
        Src/GUI/BoardWidget.h
        Src/GUI/mainwindow.ui
        Src/GUI/resources.qrc
        icon.rc
)


target_link_libraries(SnakePlusPlus PRIVATE Qt6::Widgets)