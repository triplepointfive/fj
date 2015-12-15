#include <gtest/gtest.h>
#include "ast_struct.h"

TEST (Ast, empty_class) {
    std::string child_class_name;
    std::string parent_class_name;
    std::string input = "class A extends Object { }";
    pegtl::parse< fj::class_def, pegtl::nothing >(input.c_str(), input.size(), "input variable", child_class_name, parent_class_name);

    EXPECT_EQ(child_class_name, "A");
    EXPECT_EQ(parent_class_name, "Object");
}
