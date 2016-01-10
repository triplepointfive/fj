#include <gtest/gtest.h>
#include "ast_struct.h"

TEST (AST, class_header) {
    ParsedContext context;
    const std::string input = "class A extends Object ";
    bool status = pegtl::parse< fj::class_header, fj::build_grammar>(
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

    bool status = pegtl::parse< fj::property_def, fj::build_grammar>(
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

    bool status = pegtl::parse< fj::class_body, fj::build_grammar>(
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

    bool status = pegtl::parse< fj::class_body, fj::build_grammar>(
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
    bool status = pegtl::parse< fj::file, fj::build_grammar>(
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
    bool status = pegtl::parse< fj::class_def, fj::build_grammar>(
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
    bool status = pegtl::parse< fj::file, fj::build_grammar>(
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
    const std::string input = "Object prop1";
    MethodDeclaration methodDeclaration("methodName", "Object");

    bool status = pegtl::parse< fj::method_arg, fj::build_method >(
        input,
        "input variable",
        methodDeclaration
    );

    ASSERT_TRUE(status);

    Arguments args = methodDeclaration.getArgs();
    ASSERT_EQ(1, args.size());

    EXPECT_EQ("prop1", args.rbegin()->first);
    EXPECT_EQ("Object", args.rbegin()->second);
}

TEST (AST, a_list_of_method_arguments) {
    const std::string input = "Object a, Object b";
    MethodDeclaration methodDeclaration("methodName", "Object");

    bool status = pegtl::parse< fj::method_arguments, fj::build_method >(
        input,
        "input variable",
        methodDeclaration
    );

    ASSERT_TRUE(status);

    Arguments args = methodDeclaration.getArgs();
    ASSERT_EQ(2, args.size());

    EXPECT_EQ("b", args.rbegin()->first);
    EXPECT_EQ("Object", args.rbegin()->second);

    EXPECT_EQ("a", args.rend()->first);
    EXPECT_EQ("Object", args.rend()->second);
}

TEST (AST, constructor_header_with_no_arguments) {
    const std::string input = "A()";
    ConstructorBody constructorBody;

    bool status = pegtl::parse< fj::constructor_head, fj::build_constructor >(
        input,
        "input variable",
        constructorBody
    );

    ASSERT_TRUE(status);

    EXPECT_EQ(0, constructorBody.getArgs().size());
    EXPECT_EQ("A", constructorBody.getName());
}

TEST (AST, constructor_header_with_single_argument) {
    const std::string input = "A( Object fstArg )";
    ConstructorBody constructorBody;

    bool status = pegtl::parse< fj::constructor_head, fj::build_constructor >(
        input,
        "input variable",
        constructorBody
    );

    ASSERT_TRUE(status);
    EXPECT_EQ("A", constructorBody.getName());

    Arguments args = constructorBody.getArgs();
    EXPECT_EQ(1, constructorBody.getArgs().size());
    EXPECT_EQ("fstArg", args.rbegin()->first);
    EXPECT_EQ("Object", args.rbegin()->second);
}

TEST (AST, method_header_with_no_arguments) {
    const std::string input = "Object fun1()";
    MethodDeclaration methodDeclaration("methodName", "Object");

    bool status = pegtl::parse< fj::method_head, fj::build_method >(
        input,
        "input variable",
        methodDeclaration
    );

    ASSERT_TRUE(status);

    EXPECT_EQ(0, methodDeclaration.getArgs().size());
    EXPECT_EQ("fun1", methodDeclaration.getName());
    EXPECT_EQ("Object", methodDeclaration.getReturnClassName());
}

TEST (AST, method_header_with_single_argument) {
    const std::string input = "Object fun2( Object fstArg )";
    MethodDeclaration methodDeclaration("methodName", "Object");

    bool status = pegtl::parse< fj::method_head, fj::build_method >(
        input,
        "input variable",
        methodDeclaration
    );

    ASSERT_TRUE(status);

    EXPECT_EQ("fun2", methodDeclaration.getName());
    EXPECT_EQ("Object", methodDeclaration.getReturnClassName());

    Arguments args = methodDeclaration.getArgs();
    ASSERT_EQ(1, args.size());

    EXPECT_EQ("fstArg", args.rbegin()->first);
    EXPECT_EQ("Object", args.rbegin()->second);
}

TEST (AST, constructor_body_super_invocation) {
    ConstructorBody constructorBody;
    const std::string input = "super ( )";

    bool status = pegtl::parse< fj::super_invocation, fj::build_constructor>(
        input,
        "input variable",
        constructorBody
    );

    ASSERT_TRUE(status);
    // Nothing to test here - no action applied for that rule.
}

TEST (AST, constructor_body_assignment) {
    ConstructorBody constructorBody;
    const std::string input = "this.fst= fst";

    bool status = pegtl::parse< fj::assignment, fj::build_constructor>(
        input,
        "input variable",
        constructorBody
    );

    ASSERT_TRUE(status);

    auto properties = constructorBody.getProperties();
    ASSERT_EQ(1, properties.size());
    EXPECT_EQ("fst", properties.front());
}

TEST (AST, constructor_body_with_few_assignments) {
    ConstructorBody constructorBody;
    const std::string input = "super();\n  this.snd =snd; this.fst= fst;";

    bool status = pegtl::parse< fj::constructor_body, fj::build_constructor>(
        input,
        "input variable",
        constructorBody
    );

    ASSERT_TRUE(status);

    auto properties = constructorBody.getProperties();
    ASSERT_EQ(2, properties.size());
    EXPECT_EQ("snd", properties.front());
    EXPECT_EQ("fst", properties.back());
}

TEST (AST, constructor_empty_definition) {
    ConstructorBody constructorBody;
    const std::string input = "A() { super(); }";

    bool status = pegtl::parse< fj::constructor_def, fj::build_constructor>(
        input,
        "input variable",
        constructorBody
    );

    ASSERT_TRUE(status);

    auto properties = constructorBody.getProperties();
    ASSERT_EQ(0, properties.size());
}

TEST (AST, constructor_with_an_argument_definition) {
    ConstructorBody constructorBody;
    const std::string input = "A(Object fst) { super(); this.fst = fst; }";

    bool status = pegtl::parse< fj::constructor_def, fj::build_constructor>(
        input,
        "input variable",
        constructorBody
    );

    ASSERT_TRUE(status);

    auto properties = constructorBody.getProperties();
    ASSERT_EQ(1, properties.size());
    EXPECT_EQ("fst", properties.front());
}

TEST (AST, method_returns_property) {
    MethodDeclaration methodDeclaration("methodName", "Object");
    const std::string input = "return this.fst;";

    bool status = pegtl::parse< fj::method_body, fj::build_method>(
        input,
        "input variable",
        methodDeclaration
    );

    ASSERT_TRUE(status);

    MethodTerm *methodTerm = methodDeclaration.getBodyTerm();
    ASSERT_NE(nullptr, methodTerm);
    EXPECT_EQ("property", methodTerm->type());
    EXPECT_EQ("fst", methodTerm->getName());
}

TEST (AST, method_returns_input_variable) {
    MethodDeclaration methodDeclaration("methodName", "Object");
    const std::string input = "return fstArg;";

    bool status = pegtl::parse< fj::method_body, fj::build_method>(
        input,
        "input variable",
        methodDeclaration
    );

    ASSERT_TRUE(status);

    MethodTerm *methodTerm = methodDeclaration.getBodyTerm();
    ASSERT_NE(nullptr, methodTerm);
    EXPECT_EQ("variable", methodTerm->type());
    EXPECT_EQ("fstArg", methodTerm->getName());
}

TEST (AST, method_returns_another_method_invocation) {
    MethodDeclaration methodDeclaration("methodName", "Object");
    const std::string input = "return this.someMethod();";

    bool status = pegtl::parse< fj::method_body, fj::build_method>(
        input,
        "input variable",
        methodDeclaration
    );

    ASSERT_TRUE(status);

    MethodTerm *builtTerm = methodDeclaration.getBodyTerm();
    ASSERT_NE(nullptr, builtTerm);
    MethodInvocation * methodInvocation =
            dynamic_cast<MethodInvocation *>(builtTerm);
    ASSERT_NE(nullptr, methodInvocation);
    EXPECT_EQ("someMethod", methodInvocation->getName());
    EXPECT_EQ("this", methodInvocation->getObjectName());
}
