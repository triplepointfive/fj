#include <gtest/gtest.h>
#include "lexer/lexer.h"

using namespace pegtl;
using namespace fj;

#define PARSER_ERROR(error_message) \
    ParsedContext context; \
    try { \
        bool status = parse< fj::class_def, nothing, fj::control >( \
            input, \
            "input variable", \
            context \
        ); \
        std::cout << status; \
        FAIL(); \
    } catch(const pegtl::parse_error& err) { \
        ASSERT_STREQ(error_message, err.what()); \
    }

// Tokens

TEST (lexer_error, missed_class_name) {
    const std::string input = "class {}";
    PARSER_ERROR("input variable:1:5: Missed class name")
}

TEST (lexer_error, missed_extends_keyword) {
    const std::string input = "class A {}";
    PARSER_ERROR("input variable:1:8: Expected \"extends\" keyword")
}

TEST (lexer_error, missed_parent_class_name) {
    const std::string input = "class A extends {}";
    PARSER_ERROR("input variable:1:15: Missed class name")
}

TEST (lexer_error, keywords_as_a_class_name) {
    const std::string input = "class this extends Object { A() { super(); } Object A() { return var; } }";
    PARSER_ERROR("input variable:1:5: Missed class name")
}

TEST (lexer_error, keywords_as_inherited_class_name) {
    const std::string input = "class A extends super { A() { super(); } Object A() { return var; } }";
    PARSER_ERROR("input variable:1:15: Missed class name")
}

// Braces

TEST (lexer_error, missed_open_brace_with_constructor) {
    const std::string input = "class A extends Object A() { super(); } }";
    PARSER_ERROR("input variable:1:23: Missing open brace '{'")
}

TEST (lexer_error, missed_close_brace_with_constructor) {
    const std::string input = "class A extends Object { A() { super(); }";
    PARSER_ERROR("input variable:1:41: Missing close brace '}'")
}

TEST (lexer_error, missed_open_brace_with_property) {
    const std::string input = "class A extends Object { Object a; ";
    PARSER_ERROR("input variable:1:35: Missing close brace '}'")
}

TEST (lexer_error, missed_close_brace_with_property) {
    const std::string input = "class A extends Object { Object a; ";
    PARSER_ERROR("input variable:1:35: Missing close brace '}'")
}

// Symbols

TEST (lexer_error, missed_semicolon_with_property) {
    const std::string input = "class A extends Object { Object a }";
    PARSER_ERROR("input variable:1:33: Missing semicolon")
}

// Terms

TEST (lexer_error, missed_super_invocation) {
    const std::string input = "class A extends Object { A() {} }";
    PARSER_ERROR("input variable:1:30: Call of super is a must")
}

TEST (lexer_error, missed_method_body) {
    const std::string input = "class A extends Object { A() { super(); } Object A() {} }";
    PARSER_ERROR("input variable:1:54: Method body is missing")
}

TEST (lexer_error, missed_returned_term) {
    const std::string input = "class A extends Object { A() { super(); } Object A() { return; } }";
    PARSER_ERROR("input variable:1:61: Expected a term")
}
