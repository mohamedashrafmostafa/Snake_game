cmake_minimum_required(VERSION 4.2)
project(_idea)

set(CMAKE_CXX_STANDARD 14)

add_executable(_idea
        .gitignore
        editor.xml
        modules.xml
        Snake_game.iml
        vcs.xml
        workspace.xml
        ../Core/Position.h
        ../Core/Position.h)
