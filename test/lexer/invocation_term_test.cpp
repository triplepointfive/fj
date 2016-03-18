#include <gtest/gtest.h>
#include "lexer/parser_rules.h"

using namespace pegtl;
using namespace fj;

TEST (MethodInvocation, method_returns_another_method_invocation) {
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

    VariableTerm *term = dynamic_cast<VariableTerm *>(
        methodInvocation->getTerm().get()
    );
    EXPECT_EQ("someMethod", methodInvocation->getName());
    EXPECT_EQ("this", term->getName());
    EXPECT_EQ(0, methodInvocation->getArgs().size());
}

TEST (AST, method_returns_method_invocation_for_initiation) {
    const std::string input = "return (new A()).someMethod();";
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

    Initiation *term = dynamic_cast<Initiation *>(
        methodInvocation->getTerm().get()
    );
    EXPECT_EQ("someMethod", methodInvocation->getName());
    EXPECT_EQ("A", term->getName());
    EXPECT_EQ(0, methodInvocation->getArgs().size());
}

TEST (MethodInvocation, method_returns_another_method_invocation_with_2_input_args) {
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

    ASSERT_NE(nullptr, methodInvocation->getTerm());
    VariableTerm *term = dynamic_cast<VariableTerm *>(
        methodInvocation->getTerm().get()
    );
    EXPECT_EQ("this", term->getName());

    auto args = methodInvocation->getArgs();
    ASSERT_EQ(2, args.size());

    VariableTerm * variableTerm = dynamic_cast<VariableTerm *>(
        args.front().get()
    );
    EXPECT_EQ("var1", variableTerm->getName());

    AccessTerm * propertyTerm = dynamic_cast<AccessTerm *>(
        args.back().get()
    );
    EXPECT_EQ("fst", propertyTerm->getName());
}

TEST (MethodInvocation, method_returns_another_method_invocation_with_another_invocation) {
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

    ASSERT_NE(nullptr, methodInvocation->getTerm());
    VariableTerm *thisVariable = dynamic_cast< VariableTerm * >(
        methodInvocation->getTerm().get());
    EXPECT_EQ("this", thisVariable->getName());

    auto args = methodInvocation->getArgs();
    ASSERT_EQ(1, args.size());

    MethodInvocation * methodMethodTerm = dynamic_cast<MethodInvocation *>(
        args.front().get()
    );
    ASSERT_NE(nullptr, methodMethodTerm);
    EXPECT_EQ("anotherMethod", methodMethodTerm->getName());

    ASSERT_NE(nullptr, methodMethodTerm->getTerm());
    VariableTerm *var1Variable = dynamic_cast< VariableTerm * >(
        methodMethodTerm->getTerm().get());
    EXPECT_EQ("var1", var1Variable->getName());

    args = methodMethodTerm->getArgs();
    ASSERT_EQ(1, args.size());

    AccessTerm * propertyTerm = dynamic_cast<AccessTerm *>(
        args.back().get()
    );
    EXPECT_EQ("third", propertyTerm->getName());
}
