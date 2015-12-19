#include <gtest/gtest.h>
#include "ast_struct.h"

TEST (Ast, one_empty_class) {
    ParsedContext context;
    const std::string input = "class A extends Object { }";
    bool status = pegtl::parse< fj::class_def, fj::action >(
            input,
            "input variable",
            context
    );

    ASSERT_FALSE(status);

    ClassDeclaration *newClass = context.currentClass();

    EXPECT_EQ(1, context.getClasses().size());
    EXPECT_EQ("A", newClass->getName());
    EXPECT_EQ("Object", newClass->getParentName());
}

TEST (Ast, property) {
    ParsedContext context;
    const std::string input = "Object prop1";
    ClassDeclaration classA("A");
    classA.setParentName("Object");
    context.addClass(classA);

    bool status = pegtl::parse< fj::property_def, fj::action >(
            input,
            "input variable",
            context
    );

    ASSERT_FALSE(status);

    ClassDeclaration *newClass = context.currentClass();
    const Properties *properties = newClass->getProperties();
    EXPECT_EQ(1, context.getClasses().size());
    EXPECT_EQ("A", newClass->getName());
    EXPECT_EQ("Object", newClass->getParentName());
    EXPECT_EQ(1, properties->size());
}

TEST (Ast, class_with_few_properties) {
    ParsedContext context;
    const std::string input = "class A extends Object { Object prop1; Object prop2; }";
    bool status = pegtl::parse< fj::grammar, fj::action >(
            input,
            "input variable",
            context
    );
    ASSERT_FALSE(status);

    ClassDeclaration *newClass = context.currentClass();

    ASSERT_EQ(1, context.getClasses().size());
    ASSERT_EQ("A", newClass->getName());
    ASSERT_EQ("Object", newClass->getParentName());
}

TEST (Ast, class_inheritance) {
    ParsedContext context;
    const std::string input = "class A extends Object { }\nclass B extends A { }";
    bool status = pegtl::parse< fj::grammar, fj::action >(
            input,
            "input variable",
            context
    );
    ASSERT_FALSE(status);

    ClassDeclaration *newClass = context.currentClass();

    EXPECT_EQ(2, context.getClasses().size());
    EXPECT_EQ("B", newClass->getName());
    EXPECT_EQ("A", newClass->getParentName());
}
