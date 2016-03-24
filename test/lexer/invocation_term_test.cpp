#include <gtest/gtest.h>
#include "lexer/parser_rules.h"

using namespace pegtl;
using namespace fj;

#define TEST_PARSE(input_text) \
    const std::string input = input_text; \
    MethodState methodState; \
    \
    bool status = parse< fj::method_body, nothing, fj::control>( \
        input, \
        "input variable", \
        methodState \
    ); \
    \
    ASSERT_TRUE(status); \
    \
    auto methodTerm = methodState.methodDeclaration->getBodyTerm().get(); \
    ASSERT_NE(nullptr, methodTerm); \
    \
    MethodInvocation * methodInvocation = \
        dynamic_cast<MethodInvocation *>(methodTerm); \
    ASSERT_NE(nullptr, methodInvocation);

TEST (MethodInvocation, of_variable) {
    TEST_PARSE("return this.someMethod();")

    EXPECT_EQ(0, methodInvocation->getArgs().size());
    EXPECT_EQ("someMethod", methodInvocation->getName());

    VariableTerm *variableTerm = dynamic_cast<VariableTerm *>(
        methodInvocation->getTerm().get()
    );
    ASSERT_NE(nullptr, variableTerm);
    EXPECT_EQ("this", variableTerm->getName());
}

TEST (MethodInvocation, of_initiation) {
    TEST_PARSE("return (new A()).someMethod();")

    EXPECT_EQ(0, methodInvocation->getArgs().size());
    EXPECT_EQ("someMethod", methodInvocation->getName());

    Initiation *initiationTerm = dynamic_cast<Initiation *>(
        methodInvocation->getTerm().get()
    );
    ASSERT_NE(nullptr, initiationTerm);
    EXPECT_EQ("A", initiationTerm->getName());
}

TEST (MethodInvocation, of_type_casting) {
    TEST_PARSE("return ((A) some).someMethod();")

    EXPECT_EQ("someMethod", methodInvocation->getName());
    EXPECT_EQ(0, methodInvocation->getArgs().size());

    TypeCastingTerm * typeCastingTerm
        = dynamic_cast< TypeCastingTerm * >(methodInvocation->getTerm().get());
    ASSERT_NE(nullptr, typeCastingTerm);
    EXPECT_EQ("A", typeCastingTerm->getName());
}

TEST (MethodInvocation, of_property_of_variable) {
    TEST_PARSE("return this.first.next();")

    EXPECT_EQ("next", methodInvocation->getName());
    EXPECT_EQ(0, methodInvocation->getArgs().size());

    AccessTerm * accessTerm
        = dynamic_cast< AccessTerm * >(methodInvocation->getTerm().get());
    ASSERT_NE(nullptr, accessTerm);
    EXPECT_EQ("first", accessTerm->getName());
}

TEST (MethodInvocation, of_method_invocation_of_variable) {
    TEST_PARSE("return this.first().next();")

    EXPECT_EQ("next", methodInvocation->getName());
    EXPECT_EQ(0, methodInvocation->getArgs().size());

    MethodInvocation * methodInvocationTerm
        = dynamic_cast< MethodInvocation * >(methodInvocation->getTerm().get());
    ASSERT_NE(nullptr, methodInvocationTerm);
    EXPECT_EQ("first", methodInvocationTerm->getName());
    EXPECT_EQ(0, methodInvocationTerm->getArgs().size());
}

TEST (MethodInvocation, with_variable_and_property_args) {
    TEST_PARSE("return this.someMethod(var1, this.fst);")

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

TEST (MethodInvocation, with_method_invocation_arg) {
    TEST_PARSE("return this.someMethod(var1.anotherMethod(this.third));")

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
