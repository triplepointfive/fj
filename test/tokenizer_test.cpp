#include <gtest/gtest.h>
#include "ast_struct.h"

TEST (Ast, one_empty_class) {
    ParsedContext context;
    std::string input = "class A extends Object { }";
    pegtl::parse< fj::class_def, fj::action >(input.c_str(),
                                              input.size(),
                                              "input variable",
                                              context);

    ClassDeclaration *newClass = context.currentClass();

    EXPECT_EQ(1, context.getClasses().size());
    EXPECT_EQ("A", newClass->getName());
    EXPECT_EQ("Object", newClass->getParentName());
}

TEST (Ast, property) {
    ParsedContext context;
    std::string input = "Object prop1";
    pegtl::parse< fj::property_def, fj::action >(input.c_str(),
                                                 input.size(),
                                                 "input variable",
                                                 context);
}

TEST (Ast, class_with_few_properties) {
    ParsedContext context;
    std::string input = "class A extends Object { Object prop1; Object prop2; }";
    pegtl::parse< fj::grammar, fj::action >(input.c_str(),
                                            input.size(),
                                            "input variable",
                                            context);

    ClassDeclaration *newClass = context.currentClass();

    ASSERT_EQ(1, context.getClasses().size());
    ASSERT_EQ("A", newClass->getName());
    ASSERT_EQ("Object", newClass->getParentName());
}

TEST (Ast, class_inheritance) {
    ParsedContext context;
    std::string input = "class A extends Object { }\nclass B extends A { }";
    pegtl::parse< fj::grammar, fj::action >(input.c_str(),
                                            input.size(),
                                            "input variable",
                                            context);

    ClassDeclaration *newClass = context.currentClass();

    EXPECT_EQ(2, context.getClasses().size());
    EXPECT_EQ("B", newClass->getName());
    EXPECT_EQ("A", newClass->getParentName());
}
