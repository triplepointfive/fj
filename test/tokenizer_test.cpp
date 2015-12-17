#include <gtest/gtest.h>
#include "ast_struct.h"

TEST (Ast, empty_class) {
    ParsedContext context;
//    std::string input = "class A extends Object {}";
    std::string input = "class A extends Object ";
    pegtl::parse< fj::class_def, fj::action >(input.c_str(), input.size(), "input variable", context);

    ClassDeclaration *newClass = context.currentClass();
    EXPECT_EQ("A", newClass->getName());
    EXPECT_EQ("Object", newClass->getParentName());
}
