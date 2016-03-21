cmake_minimum_required(VERSION 2.8)

option(test "Build all tests." OFF)
project(fj)

find_package(Boost 1.60.0)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -pedantic -Wextra -g -Wnon-virtual-dtor")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} --coverage")

add_library(fj_lib
        src/context.cpp
        src/eval_term_visitor.cpp
        src/utils.cpp
        src/ast/class_body.cpp
        src/ast/class_table.cpp
        src/ast/context_builder.cpp
        src/ast/method_body.cpp
        src/ast/term.cpp
        src/ast/lexer_term_builder.cpp
        src/lexer/errors.cpp
        src/lexer/structures.cpp
        src/lexer/lexer.cpp
        src/lexer/parsing_structures.cpp
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
        includes/lexer/parsing_structures.h
        includes/lexer/parser_rules.h
        includes/lexer/lexer.h
        includes/lexer/structures.h)
add_executable(fj
        src/interactive/mode.cpp
        src/interactive/help_command.cpp
        src/interactive/info_command.cpp
        src/interactive/import_command.cpp
        includes/interactive/mode.h
        includes/interactive/command.h
        includes/interactive/help_command.h
        includes/interactive/info_command.h
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
        test/lexer/access_term_test.cpp
        test/lexer/invocation_term_test.cpp
        test/term_builder_test.cpp)

target_compile_options(fj_test PUBLIC -fprofile-arcs -ftest-coverage)

include_directories(PEGTL)
include_directories(src)
include_directories(includes)

if(Boost_FOUND)
  include_directories(${Boost_INCLUDE_DIRS})
endif()

target_link_libraries(fj readline)
target_link_libraries(fj_test gtest gtest_main pthread)

target_link_libraries(fj_test fj_lib)

enable_testing()
add_test(Tests fj_test)
