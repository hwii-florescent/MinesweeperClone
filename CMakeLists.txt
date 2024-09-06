cmake_minimum_required(VERSION 3.26)
project(minesweeper)

set(CMAKE_CXX_STANDARD 17)

add_executable(minesweeper
        Button.cpp
        Button.h
        GameState.cpp
        GameState.h
        minesweeper.cpp
        minesweeper.h
        Tile.cpp
        Tile.h
        Toolbox.cpp
        Toolbox.h
        MineTile.cpp
        MineTile.h
)

set(SFML_STATIC_LIBRARIES TRUE)
set(SFML_DIR "C:/Libraries/SFML-2.5.1/lib/cmake/SFML")
find_package(SFML 2.5 COMPONENTS graphics audio REQUIRED)

include_directories(.)
target_link_libraries(minesweeper sfml-graphics sfml-audio)