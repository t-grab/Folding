cmake_minimum_required(VERSION 3.3)
project(Folding)

include_directories(E:/Development/Projekte/Evo/src/hpp)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(CMAKE_BINARY_DIR "${CMAKE_CURRENT_SOURCE_DIR}/bin")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY  "${CMAKE_CURRENT_SOURCE_DIR}/bin")
set(SOURCE_FILES main.cpp Folding.hpp Folding.cpp Grid.hpp Grid.cpp Matrix.hpp algorithm.hpp algorithm.cpp Diversity.hpp Diversity.cpp)

add_executable(Folding ${SOURCE_FILES})