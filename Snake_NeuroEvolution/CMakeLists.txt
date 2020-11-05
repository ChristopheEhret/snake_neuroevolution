cmake_minimum_required(VERSION 3.0)

project(Snake_NE)

#add_subdirectory(NeuralNetwork)

set(SRCS 
    Snake_Game.cpp
    Snake.cpp
    Snake_NeuroEvolution.cpp
)

set(HEADERS 
    stdafx.h
    Snake_Game.h
    Snake.h
)

find_package(SFML COMPONENTS graphics window system REQUIRED)
# find_package(lib_RN REQUIRED)

add_executable(snake_NE ${SRCS} ${HEADERS})

target_link_libraries(snake_NE sfml-graphics sfml-window sfml-system)
target_link_libraries(snake_NE lib_RN)