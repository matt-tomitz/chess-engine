cmake_minimum_required(VERSION 3.10.2)
project(chess
    LANGUAGES CXX
    DESCRIPTION "chess")   

add_executable(chess
    src/main.cpp
    src/helper.cpp
    src/position.cpp
    src/magic.cpp
    src/perft.cpp
    src/moves.cpp)

target_include_directories(chess PRIVATE ./inc)
target_compile_features(chess PRIVATE cxx_std_17)

target_compile_options(chess PRIVATE -Wall -Wextra -Wpedantic)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug)
  # set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")