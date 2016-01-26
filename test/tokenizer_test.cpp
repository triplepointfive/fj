#include <gtest/gtest.h>
#include "lexer/lexer.h"

using namespace pegtl;
using namespace fj;

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

    ClassState classState;

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

    ClassState classState;
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
    ClassState classState;
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
    ClassState classState;
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
    MethodState methodState;

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
    MethodState methodState;

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
    ConstructorState constructorState;

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
    ConstructorState constructorState;

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
    MethodState methodState;

    bool status = parse< fj::method_head, fj::build_method, fj::control >(
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
    MethodState methodState;

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
    ConstructorState constructorState;
    const std::string input = "super ( )";

    bool status = parse< fj::super_invocation, fj::build_constructor, fj::control>(
        input,
        "input variable",
        constructorState
    );

    ASSERT_TRUE(status);
    // Nothing to test here - no action applied for that rule.
}

TEST (AST, constructor_body_assignment) {
    const std::string input = "this.fst= fst";
    ConstructorState constructorState;

    bool status = parse< fj::assignment, fj::build_constructor, fj::control>(
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
    ConstructorState constructorState;

    bool status = parse< fj::constructor_body, fj::build_constructor, fj::control>(
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
    ClassState classState;

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
    ClassState classState;

    bool status = parse< fj::constructor_def, nothing, fj::control>(
        input,
        "input variable",
        classState
    );

    ASSERT_TRUE(status);

    auto classDeclaration = classState.classDeclaration;
    auto args = classDeclaration->getConstructorBody()->getSuperArgs();
    EXPECT_EQ(0, args.size());

    auto properties = classDeclaration->getConstructorBody()->getProperties();
    ASSERT_EQ(1, properties.size());
    EXPECT_EQ("fst", properties.front());
}

TEST (AST, constructor_with_a_super_argument) {
    const std::string input = "A(Object fst) { super(fst); }";
    ClassState classState;

    bool status = parse< fj::constructor_def, nothing, fj::control>(
        input,
        "input variable",
        classState
    );

    ASSERT_TRUE(status);

    auto classDeclaration = classState.classDeclaration;
    auto properties = classDeclaration->getConstructorBody()->getProperties();
    EXPECT_EQ(0, properties.size());

    auto args = classDeclaration->getConstructorBody()->getSuperArgs();
    ASSERT_EQ(1, args.size());

    EXPECT_EQ("fst", args.front());
}

TEST (AST, method_returns_property) {
    const std::string input = "return this.fst;";
    MethodState methodState;

    bool status = parse< fj::method_body, nothing, fj::control>(
        input,
        "input variable",
        methodState
    );

    ASSERT_TRUE(status);

    auto methodDeclaration = methodState.methodDeclaration;
    auto methodTerm = methodDeclaration->getBodyTerm().get();
    ASSERT_NE(nullptr, methodTerm);

    PropertyTerm * propertyTerm = dynamic_cast<PropertyTerm *>(methodTerm);
    EXPECT_EQ("fst", propertyTerm->getName());
}

TEST (AST, method_returns_input_variable) {
    const std::string input = "return fstArg;";
    MethodState methodState;

    bool status = parse< fj::method_body, fj::build_method, fj::control>(
        input,
        "input variable",
        methodState
    );

    ASSERT_TRUE(status);

    auto methodDeclaration = methodState.methodDeclaration;
    auto methodTerm = methodDeclaration->getBodyTerm().get();
    ASSERT_NE(nullptr, methodTerm);

    VariableTerm * variableTerm = dynamic_cast<VariableTerm *>(methodTerm);
    EXPECT_EQ("fstArg", variableTerm->getName());
}

TEST (AST, method_returns_another_method_invocation) {
    const std::string input = "return this.someMethod();";
    MethodState methodState;

    bool status = parse< fj::method_body, fj::build_method, fj::control>(
        input,
        "input variable",
        methodState
    );

    ASSERT_TRUE(status);

    auto methodDeclaration = methodState.methodDeclaration;
    auto methodTerm = methodDeclaration->getBodyTerm().get();
    ASSERT_NE(nullptr, methodTerm);

    MethodInvocation * methodInvocation =
        dynamic_cast<MethodInvocation *>(methodTerm);
    EXPECT_EQ("someMethod", methodInvocation->getName());
    EXPECT_EQ("this", methodInvocation->getObjectName());
    EXPECT_EQ(0, methodInvocation->getArgs().size());
}

TEST (AST, method_returns_another_method_invocation_with_2_input_args) {
    const std::string input = "return this.someMethod(var1, this.fst);";
    MethodState methodState;

    bool status = parse< fj::method_body, fj::build_method, fj::control>(
        input,
        "input variable",
        methodState
    );

    ASSERT_TRUE(status);

    auto methodDeclaration = methodState.methodDeclaration;
    auto methodTerm = methodDeclaration->getBodyTerm().get();
    ASSERT_NE(nullptr, methodTerm);
    MethodInvocation * methodInvocation =
            dynamic_cast<MethodInvocation *>(methodTerm);
    ASSERT_NE(nullptr, methodInvocation);
    EXPECT_EQ("someMethod", methodInvocation->getName());
    EXPECT_EQ("this", methodInvocation->getObjectName());

    auto args = methodInvocation->getArgs();
    ASSERT_EQ(2, args.size());

    VariableTerm * variableTerm = dynamic_cast<VariableTerm *>(
        args.front().get()
    );
    EXPECT_EQ("var1", variableTerm->getName());

    PropertyTerm * propertyTerm = dynamic_cast<PropertyTerm *>(
        args.back().get()
    );
    EXPECT_EQ("fst", propertyTerm->getName());
}

TEST (AST, method_returns_another_method_invocation_with_another_invocation) {
    const std::string input = "return this.someMethod(\n"
        "var1.anotherMethod(this.third));";
    MethodState methodState;

    bool status = parse< fj::method_body, fj::build_method, fj::control>(
        input,
        "input variable",
        methodState
    );

    ASSERT_TRUE(status);

    auto methodDeclaration = methodState.methodDeclaration;
    auto methodTerm = methodDeclaration->getBodyTerm().get();
    ASSERT_NE(nullptr, methodTerm);
    MethodInvocation * methodInvocation =
        dynamic_cast<MethodInvocation *>(methodTerm);
    ASSERT_NE(nullptr, methodInvocation);
    EXPECT_EQ("someMethod", methodInvocation->getName());
    EXPECT_EQ("this", methodInvocation->getObjectName());

    auto args = methodInvocation->getArgs();
    ASSERT_EQ(1, args.size());

    MethodInvocation * methodMethodTerm = dynamic_cast<MethodInvocation *>(
        args.front().get()
    );
    ASSERT_NE(nullptr, methodMethodTerm);
    EXPECT_EQ("anotherMethod", methodMethodTerm->getName());
    EXPECT_EQ("var1", methodMethodTerm->getObjectName());

    args = methodMethodTerm->getArgs();
    ASSERT_EQ(1, args.size());

    PropertyTerm * propertyTerm = dynamic_cast<PropertyTerm *>(
        args.back().get()
    );
    EXPECT_EQ("third", propertyTerm->getName());
}

TEST (AST, method_returns_initiated_object) {
    const std::string input = "return new A();";
    MethodState methodState;

    bool status = parse< fj::method_body, fj::build_method, fj::control>(
        input,
        "input variable",
        methodState
    );

    ASSERT_TRUE(status);

    auto methodDeclaration = methodState.methodDeclaration;
    auto methodTerm = methodDeclaration->getBodyTerm().get();
    ASSERT_NE(nullptr, methodTerm);

    Initiation * initiation =
        dynamic_cast<Initiation *>(methodTerm);
    EXPECT_EQ("A", initiation->getName());
    EXPECT_EQ(0, initiation->getArgs().size());
}

TEST (AST, method_returns_initiated_object_with_few_args) {
    const std::string input = "return new Point(x, this.y);";
    MethodState methodState;

    bool status = parse< fj::method_body, fj::build_method, fj::control>(
        input,
        "input variable",
        methodState
    );

    ASSERT_TRUE(status);

    auto methodDeclaration = methodState.methodDeclaration;
    auto methodTerm = methodDeclaration->getBodyTerm().get();
    ASSERT_NE(nullptr, methodTerm);

    Initiation * initiation =
        dynamic_cast<Initiation *>(methodTerm);
    auto args = initiation->getArgs();
    EXPECT_EQ("Point", initiation->getName());
    EXPECT_EQ(2, args.size());

    VariableTerm * variableTerm = dynamic_cast<VariableTerm *>(
        args.front().get()
    );
    ASSERT_NE(nullptr, variableTerm);
    EXPECT_EQ("x", variableTerm->getName());

    PropertyTerm * propertyTerm = dynamic_cast<PropertyTerm *>(
        args.back().get()
    );
    EXPECT_EQ("y", propertyTerm->getName());
}
