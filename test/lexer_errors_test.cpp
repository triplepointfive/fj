#include <gtest/gtest.h>
#include "lexer/lexer.h"

using namespace pegtl;

TEST (AST, empty_file) {
    ParsedContext context;
    const std::string input = "";

    try {
        parse< fj::grammar, nothing, fj::control >(
            input,
            "input variable",
            context
        );
        FAIL();
    } catch(const pegtl::parse_error& err) {
        ASSERT_STREQ("input variable:1:0: Empty file", err.what());
    }
}
