cmake_minimum_required(VERSION 3.3)

option(test "Build all tests." OFF)
project(fj)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -pedantic -Wextra -g")

add_library(fj_lib src/term.cpp src/term_visitor.h src/eval_term_visitor.cpp src/eval_term_visitor.h src/utils.h src/utils.cpp src/context.cpp src/context.h test/main_test.cpp src/method_body.cpp src/method_body.h src/class_body.cpp src/class_body.h src/base_types.h)
add_executable(fj src/main.cpp)
target_link_libraries(fj fj_lib)

add_executable(fj_test test/main_test.cpp)
target_link_libraries(fj_test gtest gtest_main pthread)
target_link_libraries(fj_test fj_lib)
