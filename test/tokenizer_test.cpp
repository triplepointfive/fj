#include <gtest/gtest.h>
#include "lexer/lexer.h"

using namespace pegtl;

TEST (AST, file_containing_a_class) {
    ParsedContext context;
    const std::string input = "class A extends Object { Object a; }";
    bool status = parse< fj::grammar, nothing, fj::control >(
        input,
        "input variable",
        context
    );

    ASSERT_TRUE(status);
    auto classes = context.getClasses();
    ASSERT_EQ(1, classes.size());

    auto newClass = classes.front();
    EXPECT_EQ("A", newClass->getName());
    EXPECT_EQ("Object", newClass->getParentName());

    const Properties *properties = newClass->getProperties();
    ASSERT_EQ(1, properties->size());
    EXPECT_EQ("a", properties->begin()->first);
    EXPECT_EQ("Object", properties->begin()->second);
}

TEST (AST, class_header) {
    const std::string input = "class A extends Object ";

    fj::ClassState classState;

    bool status = parse< fj::class_header, fj::build_class, fj::control>(
        input,
        "input variable",
        classState
    );

    auto classDeclaration = classState.classDeclaration;

    ASSERT_TRUE(status);
    EXPECT_EQ("A", classDeclaration->getName());
    EXPECT_EQ("Object", classDeclaration->getParentName());
}

TEST (AST, single_property_definition) {
    const std::string input = "Object prop1;";

    fj::ClassState classState;
    classState.classDeclaration->setName("A");
    classState.classDeclaration->setParentName("Object");

    bool status = parse< fj::property_def, fj::build_class, fj::control >(
        input,
        "input variable",
        classState
    );

    ASSERT_TRUE(status);

    auto classA = classState.classDeclaration;
    const Properties *properties = classA->getProperties();
    EXPECT_EQ(1, properties->size());
}

TEST (AST, class_body_with_a_single_property) {
    const std::string input = "Object prop1;";
    fj::ClassState classState;
    classState.classDeclaration->setName("A");
    classState.classDeclaration->setParentName("Object");

    bool status = parse< fj::class_body, fj::build_class, fj::control >(
        input,
        "input variable",
        classState
    );

    ASSERT_TRUE(status);

    auto classA = classState.classDeclaration;
    const Properties *properties = classA->getProperties();
    EXPECT_EQ(1, properties->size());
}

TEST (AST, class_body_with_few_properties) {
    const std::string input = "Object prop1 ; Object prop2; ";
    fj::ClassState classState;
    classState.classDeclaration->setName("A");
    classState.classDeclaration->setParentName("Object");

    bool status = parse< fj::class_body, fj::build_class, fj::control >(
        input,
        "input variable",
        classState
    );

    ASSERT_TRUE(status);

    auto classA = classState.classDeclaration;
    const Properties *properties = classA->getProperties();
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
    bool status = parse< fj::grammar, nothing, fj::control >(
        input,
        "input variable",
        context
    );

    ASSERT_TRUE(status);
    auto classes = context.getClasses();
    ASSERT_EQ(1, classes.size());

    auto newClass = classes.front();
    EXPECT_EQ("A", newClass->getName());
    EXPECT_EQ("Object", newClass->getParentName());
}

TEST (AST, class_with_few_properties) {
    ParsedContext context;
    const std::string input = "class A extends Object { Object prop1; Object prop2; }";
    bool status = parse< fj::grammar, nothing, fj::control >(
        input,
        "input variable",
        context
    );
    ASSERT_TRUE(status);

    auto classes = context.getClasses();
    ASSERT_EQ(1, classes.size());

    auto newClass = classes.front();
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

TEST (AST, class_inheritance) {
    ParsedContext context;
    const std::string input = "class A extends Object { Object prop1;}"
            "\nclass B extends A { Object prop2;}";
    bool status = parse< fj::grammar, nothing, fj::control >(
        input,
        "input variable",
        context
    );
    ASSERT_TRUE(status);

    auto classes = context.getClasses();
    ASSERT_EQ(2, classes.size());

    auto newClass = classes.back();

    EXPECT_EQ("B", newClass->getName());
    EXPECT_EQ("A", newClass->getParentName());
}

TEST (AST, single_method_argument) {
    const std::string input = "Object prop1";
    fj::MethodState methodState;

    bool status = parse< fj::method_arg, fj::build_method, fj::control >(
        input,
        "input variable",
        methodState
    );

    ASSERT_TRUE(status);

    auto methodDeclaration = methodState.methodDeclaration;
    Arguments args = methodDeclaration->getArgs();
    ASSERT_EQ(1, args.size());

    EXPECT_EQ("prop1", args.rbegin()->first);
    EXPECT_EQ("Object", args.rbegin()->second);
}

TEST (AST, a_list_of_method_arguments) {
    const std::string input = "Object a, Object b";
    fj::MethodState methodState;

    bool status = parse< fj::method_arguments, fj::build_method, fj::control >(
        input,
        "input variable",
        methodState
    );

    ASSERT_TRUE(status);

    auto methodDeclaration = methodState.methodDeclaration;
    Arguments args = methodDeclaration->getArgs();
    ASSERT_EQ(2, args.size());

    EXPECT_EQ("b", args.rbegin()->first);
    EXPECT_EQ("Object", args.rbegin()->second);

    EXPECT_EQ("a", args.rend()->first);
    EXPECT_EQ("Object", args.rend()->second);
}

TEST (AST, constructor_header_with_no_arguments) {
    const std::string input = "A()";
    fj::ConstructorState constructorState;

    bool status = parse< fj::constructor_head, fj::build_constructor >(
        input,
        "input variable",
        constructorState
    );

    ASSERT_TRUE(status);

    auto constructorBody = constructorState.constructorBody;
    EXPECT_EQ(0, constructorBody->getArgs().size());
    EXPECT_EQ("A", constructorBody->getName());
}

TEST (AST, constructor_header_with_single_argument) {
    const std::string input = "A( Object fstArg )";
    fj::ConstructorState constructorState;

    bool status = parse< fj::constructor_head, fj::build_constructor, fj::control >(
        input,
        "input variable",
        constructorState
    );

    ASSERT_TRUE(status);

    auto constructorBody = constructorState.constructorBody;
    EXPECT_EQ("A", constructorBody->getName());

    Arguments args = constructorBody->getArgs();
    ASSERT_EQ(1, args.size());
    EXPECT_EQ("fstArg", args.rbegin()->first);
    EXPECT_EQ("Object", args.rbegin()->second);
}

TEST (AST, method_header_with_no_arguments) {
    const std::string input = "Object fun1()";
    fj::MethodState methodState;

    bool status = parse< fj::method_head, fj::build_method >(
        input,
        "input variable",
        methodState
    );

    ASSERT_TRUE(status);

    auto methodDeclaration = methodState.methodDeclaration;
    EXPECT_EQ(0, methodDeclaration->getArgs().size());
    EXPECT_EQ("fun1", methodDeclaration->getName());
    EXPECT_EQ("Object", methodDeclaration->getReturnClassName());
}

TEST (AST, method_header_with_single_argument) {
    const std::string input = "Object fun2( Object fstArg )";
    fj::MethodState methodState;

    bool status = parse< fj::method_head, fj::build_method, fj::control >(
        input,
        "input variable",
        methodState
    );

    ASSERT_TRUE(status);

    auto methodDeclaration = methodState.methodDeclaration;
    EXPECT_EQ("fun2", methodDeclaration->getName());
    EXPECT_EQ("Object", methodDeclaration->getReturnClassName());

    Arguments args = methodDeclaration->getArgs();
    ASSERT_EQ(1, args.size());

    EXPECT_EQ("fstArg", args.rbegin()->first);
    EXPECT_EQ("Object", args.rbegin()->second);
}

TEST (AST, constructor_body_super_invocation) {
    ConstructorBody constructorBody;
    const std::string input = "super ( )";

    bool status = parse< fj::super_invocation, fj::build_constructor>(
        input,
        "input variable",
        constructorBody
    );

    ASSERT_TRUE(status);
    // Nothing to test here - no action applied for that rule.
}

TEST (AST, constructor_body_assignment) {
    const std::string input = "this.fst= fst";
    fj::ConstructorState constructorState;

    bool status = parse< fj::assignment, fj::build_constructor>(
        input,
        "input variable",
        constructorState
    );

    ASSERT_TRUE(status);

    auto constructorBody = constructorState.constructorBody;
    auto properties = constructorBody->getProperties();
    ASSERT_EQ(1, properties.size());
    EXPECT_EQ("fst", properties.front());
}

TEST (AST, constructor_body_with_few_assignments) {
    const std::string input = "super();\n  this.snd =snd; this.fst= fst;";
    fj::ConstructorState constructorState;

    bool status = parse< fj::constructor_body, fj::build_constructor>(
        input,
        "input variable",
        constructorState
    );

    ASSERT_TRUE(status);

    auto constructorBody = constructorState.constructorBody;
    auto properties = constructorBody->getProperties();
    ASSERT_EQ(2, properties.size());
    EXPECT_EQ("snd", properties.front());
    EXPECT_EQ("fst", properties.back());
}

TEST (AST, constructor_empty_definition) {
    const std::string input = "A() { super(); }";
    fj::ClassState classState;

    bool status = parse< fj::constructor_def, nothing, fj::control>(
        input,
        "input variable",
        classState
    );

    ASSERT_TRUE(status);

    auto classDeclaration = classState.classDeclaration;
    auto properties = classDeclaration->getConstructorBody()->getProperties();
    ASSERT_EQ(0, properties.size());
}

TEST (AST, constructor_with_an_argument_definition) {
    const std::string input = "A(Object fst) { super(); this.fst = fst; }";
    fj::ClassState classState;

    bool status = parse< fj::constructor_def, nothing, fj::control>(
        input,
        "input variable",
        classState
    );

    ASSERT_TRUE(status);

    auto classDeclaration = classState.classDeclaration;
    auto properties = classDeclaration->getConstructorBody()->getProperties();
    ASSERT_EQ(1, properties.size());
    EXPECT_EQ("fst", properties.front());
}

TEST (AST, method_returns_property) {
    const std::string input = "return this.fst;";
    fj::MethodState methodState;

    bool status = parse< fj::method_body, fj::build_method>(
        input,
        "input variable",
        methodState
    );

    ASSERT_TRUE(status);

    auto methodDeclaration = methodState.methodDeclaration;
    PropertyTerm * propertyTerm =
        dynamic_cast<PropertyTerm *>(methodDeclaration->getBodyTerm());
    ASSERT_NE(nullptr, propertyTerm);
    EXPECT_EQ("fst", propertyTerm->getName());
}

TEST (AST, method_returns_input_variable) {
    const std::string input = "return fstArg;";
    fj::MethodState methodState;

    bool status = parse< fj::method_body, fj::build_method>(
        input,
        "input variable",
        methodState
    );

    ASSERT_TRUE(status);

    auto methodDeclaration = methodState.methodDeclaration;
    VariableTerm * variableTerm =
        dynamic_cast<VariableTerm *>(methodDeclaration->getBodyTerm());
    ASSERT_NE(nullptr, variableTerm);
    EXPECT_EQ("fstArg", variableTerm->getName());
}

TEST (AST, method_returns_another_method_invocation) {
    const std::string input = "return this.someMethod();";
    fj::MethodState methodState;

    bool status = parse< fj::method_body, fj::build_method>(
        input,
        "input variable",
        methodState
    );

    ASSERT_TRUE(status);

    auto methodDeclaration = methodState.methodDeclaration;
    MethodTerm *builtTerm = methodDeclaration->getBodyTerm();
    ASSERT_NE(nullptr, builtTerm);
    MethodInvocation * methodInvocation =
            dynamic_cast<MethodInvocation *>(builtTerm);
    ASSERT_NE(nullptr, methodInvocation);
    EXPECT_EQ("someMethod", methodInvocation->getName());
    EXPECT_EQ("this", methodInvocation->getObjectName());
    EXPECT_EQ(0, methodInvocation->getArgs()->size());
}

TEST (AST, DISABLED_method_returns_another_method_invocation_with_2_input_args) {
    const std::string input = "return this.someMethod(var1, this.fst);";
    fj::MethodState methodState;

    bool status = parse< fj::method_body, fj::build_method>(
        input,
        "input variable",
        methodState
    );

    ASSERT_TRUE(status);

    auto methodDeclaration = methodState.methodDeclaration;
    MethodTerm *builtTerm = methodDeclaration->getBodyTerm();
    ASSERT_NE(nullptr, builtTerm);
    MethodInvocation * methodInvocation =
            dynamic_cast<MethodInvocation *>(builtTerm);
    ASSERT_NE(nullptr, methodInvocation);
    EXPECT_EQ("someMethod", methodInvocation->getName());
    EXPECT_EQ("this", methodInvocation->getObjectName());

    auto args = methodInvocation->getArgs();
    ASSERT_EQ(2, args->size());
}
