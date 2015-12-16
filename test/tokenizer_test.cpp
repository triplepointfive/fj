#include <gtest/gtest.h>
#include "ast_struct.h"

TEST (Ast, empty_class) {
    std::string child_class_name;
    std::string parent_class_name;
//    std::string input = "class A extends Object {}";
    std::string input = "class A ";
    pegtl::parse< fj::class_def, fj::action >(input.c_str(), input.size(), "input variable", parent_class_name);

    EXPECT_EQ("A", child_class_name);
    EXPECT_EQ("Object", parent_class_name);
}
