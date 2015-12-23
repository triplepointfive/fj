#include <gtest/gtest.h>
#include "ast_struct.h"

TEST (Ast, class_header) {
    ParsedContext context;
    const std::string input = "class A extends Object ";
    bool status = pegtl::parse< fj::class_header, fj::action >(
        input,
        "input variable",
        context
    );

    ASSERT_TRUE(status);
    ASSERT_EQ(1, context.getClasses().size());

    ClassDeclaration *newClass = context.currentClass();
    EXPECT_EQ("A", newClass->getName());
    EXPECT_EQ("Object", newClass->getParentName());
}

TEST (AST, single_property_definition) {
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

    ASSERT_TRUE(status);

    ClassDeclaration *newClass = context.currentClass();
    const Properties *properties = newClass->getProperties();
    EXPECT_EQ(1, context.getClasses().size());
    EXPECT_EQ("A", newClass->getName());
    EXPECT_EQ("Object", newClass->getParentName());
    EXPECT_EQ(1, properties->size());
}

TEST (AST, class_body_with_a_single_property) {
    ParsedContext context;
    const std::string input = "Object prop1;";
    ClassDeclaration classA("A");
    classA.setParentName("Object");
    context.addClass(classA);

    bool status = pegtl::parse< fj::class_body, fj::action >(
        input,
        "input variable",
        context
    );

    ASSERT_TRUE(status);

    ClassDeclaration *newClass = context.currentClass();
    const Properties *properties = newClass->getProperties();
    EXPECT_EQ(1, context.getClasses().size());
    EXPECT_EQ("A", newClass->getName());
    EXPECT_EQ("Object", newClass->getParentName());
    EXPECT_EQ(1, properties->size());
}

TEST (AST, class_body_with_few_properties) {
    ParsedContext context;
    const std::string input = "Object prop1 ; Object prop2; ";
    ClassDeclaration classA("A");
    classA.setParentName("Object");
    context.addClass(classA);

    bool status = pegtl::parse< fj::class_body, fj::action >(
        input,
        "input variable",
        context
    );

    ASSERT_TRUE(status);

    ClassDeclaration *newClass = context.currentClass();
    EXPECT_EQ(1, context.getClasses().size());
    EXPECT_EQ("A", newClass->getName());
    EXPECT_EQ("Object", newClass->getParentName());

    const Properties *properties = newClass->getProperties();
    ASSERT_EQ(2, properties->size());

    auto firstProperty = properties->begin();
    EXPECT_EQ("prop1", firstProperty->first);
    EXPECT_EQ("Object", firstProperty->second);

    auto lastProperty = properties->rbegin();
    EXPECT_EQ("prop2", lastProperty->first);
    EXPECT_EQ("Object", lastProperty->second);
}

TEST (AST, single_class_with_one_one_property) {
    ParsedContext context;
    const std::string input = "class A extends Object { Object prop1; }";
    bool status = pegtl::parse< fj::file, fj::action >(
        input,
        "input variable",
        context
    );

    ASSERT_TRUE(status);
    ASSERT_EQ(1, context.getClasses().size());

    ClassDeclaration *newClass = context.currentClass();
    EXPECT_EQ("A", newClass->getName());
    EXPECT_EQ("Object", newClass->getParentName());
}

TEST (AST, class_with_few_properties) {
    ParsedContext context;
    const std::string input = "class A extends Object { Object prop1; Object prop2; }";
    bool status = pegtl::parse< fj::class_def, fj::action >(
        input,
        "input variable",
        context
    );
    ASSERT_TRUE(status);

    ClassDeclaration *newClass = context.currentClass();

    ASSERT_EQ(1, context.getClasses().size());
    ASSERT_EQ("A", newClass->getName());
    ASSERT_EQ("Object", newClass->getParentName());

    const Properties *properties = newClass->getProperties();
    ASSERT_EQ(2, properties->size());

    auto firstProperty = properties->begin();
    EXPECT_EQ("prop1", firstProperty->first);
    EXPECT_EQ("Object", firstProperty->second);

    auto lastProperty = properties->rbegin();
    EXPECT_EQ("prop2", lastProperty->first);
    EXPECT_EQ("Object", lastProperty->second);
}

TEST (AST, class_inheritance) {
    ParsedContext context;
    const std::string input = "class A extends Object { Object prop1;}"
            "\nclass B extends A { Object prop2;}";
    bool status = pegtl::parse< fj::file, fj::action >(
        input,
        "input variable",
        context
    );
    ASSERT_TRUE(status);

    ClassDeclaration *newClass = context.currentClass();

    EXPECT_EQ(2, context.getClasses().size());
    EXPECT_EQ("B", newClass->getName());
    EXPECT_EQ("A", newClass->getParentName());
}

TEST (AST, single_method_argument) {
    ParsedContext context;
    const std::string input = "Object prop1";
    ClassDeclaration classA("A");
    classA.setParentName("Object");
    context.addClass(classA);

    bool status = pegtl::parse< fj::method_arg, fj::action >(
        input,
        "input variable",
        context
    );

    ASSERT_TRUE(status);
}

TEST (AST, a_list_of_method_arguments) {
    ParsedContext context;
    const std::string input = "Object a, Object b";
    ClassDeclaration classA("A");
    classA.setParentName("Object");
    context.addClass(classA);

    bool status = pegtl::parse< fj::method_arguments, fj::action >(
            input,
            "input variable",
            context
    );

    ASSERT_TRUE(status);
}

TEST (AST, constructor_header_with_no_arguments) {
    ParsedContext context;
    const std::string input = "A()";
    ClassDeclaration classA("A");
    classA.setParentName("Object");
    context.addClass(classA);

    bool status = pegtl::parse< fj::method_head, fj::action >(
            input,
            "input variable",
            context
    );

    ASSERT_TRUE(status);
}

TEST (AST, constructor_header_with_single_argument) {
    ParsedContext context;
    const std::string input = "A( Object a )";
    ClassDeclaration classA("A");
    classA.setParentName("Object");
    context.addClass(classA);

    bool status = pegtl::parse< fj::method_head, fj::action >(
            input,
            "input variable",
            context
    );

    ASSERT_TRUE(status);
}
