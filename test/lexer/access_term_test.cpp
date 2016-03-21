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
    AccessTerm * propertyTerm = dynamic_cast<AccessTerm *>(methodTerm); \
    ASSERT_NE(nullptr, propertyTerm);

TEST (AccessTerm, property_of_variable) {
    TEST_PARSE("return this.fst;")

    EXPECT_EQ("fst", propertyTerm->getName());

    ASSERT_NE(nullptr, propertyTerm->getTerm());
    VariableTerm * variableTerm
        = dynamic_cast< VariableTerm * >(propertyTerm->getTerm().get());
    ASSERT_NE(nullptr, variableTerm);
    EXPECT_EQ("this", variableTerm->getName());
}

TEST (AccessTerm, property_of_initiation) {
    TEST_PARSE("return (new A()).fst;")

    EXPECT_EQ("fst", propertyTerm->getName());

    ASSERT_NE(nullptr, propertyTerm->getTerm());
    Initiation * initiationTerm
        = dynamic_cast< Initiation * >(propertyTerm->getTerm().get());
    ASSERT_NE(nullptr, initiationTerm);
    EXPECT_EQ("A", initiationTerm->getName());
}

TEST (AccessTerm, property_of_type_casting) {
    TEST_PARSE("return ((A) var1).fst;")

    EXPECT_EQ("fst", propertyTerm->getName());

    ASSERT_NE(nullptr, propertyTerm->getTerm());
    TypeCastingTerm * typeCastingTerm
        = dynamic_cast< TypeCastingTerm * >(propertyTerm->getTerm().get());
    ASSERT_NE(nullptr, typeCastingTerm);
    EXPECT_EQ("A", typeCastingTerm->getName());
}

TEST (AccessTerm, property_of_method_invocation) {
    TEST_PARSE("return var1.properties().fst;")

    EXPECT_EQ("fst", propertyTerm->getName());

    ASSERT_NE(nullptr, propertyTerm->getTerm());
    MethodInvocation * methodInvocationTerm
        = dynamic_cast< MethodInvocation * >(propertyTerm->getTerm().get());
    ASSERT_NE(nullptr, methodInvocationTerm);
    EXPECT_EQ("properties", methodInvocationTerm->getName());

    ASSERT_NE(nullptr, methodInvocationTerm->getTerm());
    VariableTerm * variableTerm
        = dynamic_cast< VariableTerm * >(methodInvocationTerm->getTerm().get());
    ASSERT_NE(nullptr, variableTerm);
    EXPECT_EQ("var1", variableTerm->getName());
}

TEST (AccessTerm, property_of_property_of_variable) {
    TEST_PARSE("return this.snd.fst;")

    EXPECT_EQ("fst", propertyTerm->getName());
    ASSERT_NE(nullptr, propertyTerm->getTerm());

    AccessTerm * internalPropertyTerm
        = dynamic_cast<AccessTerm *>(propertyTerm->getTerm().get());
    EXPECT_EQ("snd", internalPropertyTerm->getName());
    ASSERT_NE(nullptr, internalPropertyTerm->getTerm());

    VariableTerm * variableTerm
        = dynamic_cast<VariableTerm *>(internalPropertyTerm->getTerm().get());
    EXPECT_EQ("this", variableTerm->getName());
}
