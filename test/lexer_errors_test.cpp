#include <gtest/gtest.h>
#include "lexer/lexer.h"

using namespace pegtl;

#define PARSER_ERROR(error_message) \
    ParsedContext context; \
    try { \
        parse< fj::grammar, nothing, fj::control >( \
                input, \
        "input variable", \
        context \
        ); \
        FAIL(); \
    } catch(const pegtl::parse_error& err) { \
        ASSERT_STREQ(error_message, err.what()); \
    }

TEST (AST, empty_file) {
    const std::string input = "";
    PARSER_ERROR("input variable:1:0: Empty file")
}

TEST (AST, missed_class_name) {
    const std::string input = "class {}";
    PARSER_ERROR("input variable:1:5: Missed class name")
}

TEST (AST, missed_extends_keyword) {
    const std::string input = "class A {}";
    PARSER_ERROR("input variable:1:8: Expected \"extends\" keyword")
}

TEST (AST, missed_parent_class_name) {
    const std::string input = "class A extends {}";
    PARSER_ERROR("input variable:1:15: Missed class name")
}
