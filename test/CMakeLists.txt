cmake_minimum_required(VERSION 3.10.2)
project(chess
    LANGUAGES CXX
    DESCRIPTION "chess")   

add_executable(chess
    src/main.cpp
    src/helper.cpp
    src/bitboard.cpp
    src/magic.cpp
    src/moves.cpp)

target_include_directories(chess PRIVATE ./inc)
target_compile_features(chess PRIVATE cxx_std_17)

target_compile_options(chess PRIVATE -Wall -Wextra -Wpedantic)