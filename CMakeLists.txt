cmake_minimum_required(VERSION 2.8)

option(test "Build all tests." OFF)
project(fj)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -pedantic -Wextra -g")

add_library(fj_lib src/term.cpp src/term_visitor.h src/eval_term_visitor.cpp src/eval_term_visitor.h src/utils.h src/utils.cpp src/context.cpp src/context.h test/context_test.cpp src/method_body.cpp src/method_body.h src/class_body.cpp src/class_body.h src/base_types.h src/ast_struct.h)
add_executable(fj src/main.cpp src/tokenizer/keyword_token.cpp src/tokenizer/keyword_token.h src/tokenizer/tokenizer.cpp src/tokenizer/tokenizer.h src/tokenizer/token.h src/tokenizer/token_builder.cpp src/tokenizer/token_builder.h src/tokenizer/bracket_token.cpp src/tokenizer/bracket_token.h)
target_link_libraries(fj fj_lib)

add_executable(fj_test test/class_body_test.cpp test/context_test.cpp test/evaluation_test.cpp)
include_directories(../PEGTL)
include_directories(src)
target_link_libraries(fj_test gtest gtest_main pthread)
target_link_libraries(fj_test fj_lib)

