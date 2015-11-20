cmake_minimum_required(VERSION 3.3)
project(fj)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -pedantic -Wextra")

set(SOURCE_FILES src/main.cpp)
add_executable(fj ${SOURCE_FILES})