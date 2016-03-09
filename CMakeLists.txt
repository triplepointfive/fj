cmake_minimum_required(VERSION 2.8)

option(test "Build all tests." OFF)
project(fj)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -pedantic -Wextra -g -Wnon-virtual-dtor")

add_library(fj_lib
        src/context.cpp
        src/eval_term_visitor.cpp
        src/utils.cpp
        src/ast/class_body.cpp
        src/ast/context_builder.cpp
        src/ast/method_body.cpp
        src/ast/term.cpp
        src/ast/lexer_term_builder.cpp
        src/lexer/errors.cpp
        src/lexer/structures.cpp
        includes/base_types.h
        includes/context.h
        includes/eval_term_visitor.h
        includes/term_visitor.h
        includes/utils.h
        includes/ast/class_body.h
        includes/ast/class_table.h
        includes/ast/context_builder.h
        includes/ast/lexer_term_builder.h
        includes/ast/method_body.h
        includes/ast/structures.h
        includes/lexer/errors.h
        includes/lexer/lexemes.h
        includes/lexer/lexer.h
        includes/lexer/structures.h)
add_executable(fj
        src/interactive/mode.cpp
        src/interactive/help_command.cpp
        src/interactive/import_command.cpp
        includes/interactive/mode.h
        includes/interactive/command.h
        includes/interactive/help_command.h
        includes/interactive/import_command.h
        src/main.cpp)
target_link_libraries(fj fj_lib)

add_executable(fj_test
        test/class_body_test.cpp
        test/context_test.cpp
        test/evaluation_test.cpp
        test/tokenizer_test.cpp
        test/lexer_errors_test.cpp
        test/context_builder_test.cpp
        test/term_builder_test.cpp)
include_directories(PEGTL)
include_directories(src)
include_directories(includes)

target_link_libraries(fj readline)
target_link_libraries(fj_test gtest gtest_main pthread)
target_link_libraries(fj_test fj_lib)

enable_testing()
add_test(A fj_test)
