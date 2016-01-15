cmake_minimum_required(VERSION 2.8)

option(test "Build all tests." OFF)
project(fj)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -pedantic -Wextra -g -Wnon-virtual-dtor")

add_library(fj_lib
        src/term.cpp
        src/lexer/structures.cpp
        src/lexer/errors.cpp
        src/eval_term_visitor.cpp
        src/utils.cpp
        src/context.cpp
        src/method_body.cpp
        src/class_body.cpp
        includes/term_visitor.h
        includes/eval_term_visitor.h
        includes/utils.h
        includes/context.h
        includes/method_body.h
        includes/class_body.h
        includes/base_types.h
        includes/lexer/lexer.h
        includes/lexer/lexemes.h
        includes/lexer/errors.h
        includes/lexer/structures.h)
add_executable(fj src/main.cpp)
target_link_libraries(fj fj_lib)

add_executable(fj_test
        test/class_body_test.cpp
        test/context_test.cpp
        test/evaluation_test.cpp
        test/tokenizer_test.cpp
        test/lexer_errors_test.cpp)
include_directories(PEGTL)
include_directories(src)
include_directories(includes)
target_link_libraries(fj_test gtest gtest_main pthread)
target_link_libraries(fj_test fj_lib)

enable_testing()
add_test(A fj_test)
