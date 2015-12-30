cmake_minimum_required(VERSION 3.3)
project(Folding)

include_directories(E:/Development/Projekte/Evo/src/hpp)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp Folding.hpp Folding.cpp Position.hpp)
add_executable(Folding ${SOURCE_FILES})