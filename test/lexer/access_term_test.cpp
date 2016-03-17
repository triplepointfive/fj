#include <gtest/gtest.h>
#include "lexer/lexer.h"

using namespace pegtl;
using namespace fj;

TEST (AccessTerm, method_returns_anothers_object_property) {
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

    AccessTerm * propertyTerm = dynamic_cast<AccessTerm *>(methodTerm);
    EXPECT_EQ("fst", propertyTerm->getName());
}

TEST (AccessTerm, method_returns_anothers_properties_chain) {
    const std::string input = "return this.snd.fst;";
    MethodState methodState;

    bool status = parse< fj::method_body, nothing, fj::control>(
        input,
        "input variable",
        methodState
    );

    ASSERT_TRUE(status);

    auto methodTerm = methodState.methodDeclaration->getBodyTerm().get();
    ASSERT_NE(nullptr, methodTerm);

    AccessTerm * propertyTerm = dynamic_cast<AccessTerm *>(methodTerm);
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
