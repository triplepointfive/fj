#include <gtest/gtest.h>
#include "ast/builder.h"

using namespace fj;

TEST (term_builder, variable_term) {
    VariableTerm variableTerm("total");

    LexerTermBuilder builder;
    builder.visitVariableTerm(&variableTerm);
    TermPtr term = builder.getTerm();

    ASSERT_NE(nullptr, term);
    Variable * variable = dynamic_cast<Variable *>(term.get());
    ASSERT_NE(nullptr, variable);
    ASSERT_EQ("total", variable->getName().t);
}

TEST (term_builder, variable_property_term) {
    std::shared_ptr< VariableTerm > variableTerm
        = std::make_shared< VariableTerm >("queue");

    AccessTerm accessTerm;
    accessTerm.setName("last");
    accessTerm.setTerm(variableTerm);

    LexerTermBuilder builder;
    builder.visitAccessTerm(&accessTerm);
    TermPtr term = builder.getTerm();

    ASSERT_NE(nullptr, term);
    Access *property = dynamic_cast<Access *>(term.get());
    ASSERT_NE(nullptr, property);
    ASSERT_EQ("last", property->getPropertyName().t);

    ASSERT_NE(nullptr, property->getObject());
    Variable * variable = dynamic_cast<Variable *>(property->getObject().get());
    ASSERT_NE(nullptr, variable);
    ASSERT_EQ("queue", variable->getName().t);
}

TEST (term_builder, variable_coercion_term) {
    std::shared_ptr< VariableTerm > variableTerm
        = std::make_shared< VariableTerm >("queue");

    TypeCastingTerm typeCastingTerm;
    typeCastingTerm.setName("Pair");
    typeCastingTerm.setTerm(variableTerm);

    LexerTermBuilder builder;
    builder.visitTypeCastingTerm(&typeCastingTerm);
    TermPtr term = builder.getTerm();

    ASSERT_NE(nullptr, term);
    Coercion *coercion = dynamic_cast<Coercion *>(term.get());
    ASSERT_NE(nullptr, coercion);
    ASSERT_EQ("Pair", coercion->getClassName().t);

    ASSERT_NE(nullptr, coercion->getObject());
    Variable * variable = dynamic_cast<Variable *>(coercion->getObject().get());
    ASSERT_NE(nullptr, variable);
    ASSERT_EQ("queue", variable->getName().t);
}

TEST (term_builder, empty_initiation) {
    Initiation initiation;
    initiation.setName("Object");

    LexerTermBuilder builder;
    builder.visitInitiation(&initiation);
    TermPtr term = builder.getTerm();

    ASSERT_NE(nullptr, term);
    Constructor *constructor = dynamic_cast<Constructor *>(term.get());
    ASSERT_NE(nullptr, constructor);
    ASSERT_EQ("Object", constructor->getClassName().t);
    ASSERT_EQ(0, constructor->getArgs().size());
}

TEST (term_builder, initiation_with_few_args) {
    std::shared_ptr< VariableTerm > variableTerm1
        = std::make_shared< VariableTerm >("fst");
    std::shared_ptr< VariableTerm > variableTerm2
        = std::make_shared< VariableTerm >("snd");

    Initiation initiation;
    initiation.setName("Pair");
    initiation.addArg(variableTerm1);
    initiation.addArg(variableTerm2);

    LexerTermBuilder builder;
    builder.visitInitiation(&initiation);
    TermPtr term = builder.getTerm();

    ASSERT_NE(nullptr, term);
    Constructor *constructor = dynamic_cast<Constructor *>(term.get());
    ASSERT_NE(nullptr, constructor);
    EXPECT_EQ("Pair", constructor->getClassName().t);

    MethodArguments args = constructor->getArgs();
    ASSERT_EQ(2, args.size());

    Variable * variable1 = dynamic_cast<Variable *>(args.front().get());
    ASSERT_NE(nullptr, variable1);
    ASSERT_EQ("fst", variable1->getName().t);

    Variable * variable2 = dynamic_cast<Variable *>(args.back().get());
    ASSERT_NE(nullptr, variable2);
    ASSERT_EQ("snd", variable2->getName().t);
}

TEST (term_builder, method_invocation_with_arg) {
    std::shared_ptr< VariableTerm > arg
        = std::make_shared< VariableTerm >("fst");

    std::shared_ptr< VariableTerm > invocatedObject
        = std::make_shared< VariableTerm >("this");

    MethodInvocation methodInvocation;
    methodInvocation.setName("replace");
    methodInvocation.addArg(arg);
    methodInvocation.setTerm(invocatedObject);

    LexerTermBuilder builder;
    builder.visitMethodInvocation(&methodInvocation);
    TermPtr term = builder.getTerm();

    ASSERT_NE(nullptr, term);
    Invocation *invocation = dynamic_cast<Invocation *>(term.get());
    ASSERT_NE(nullptr, invocation);
    EXPECT_EQ("replace", invocation->getMethodName().t);

    Variable * object = dynamic_cast<Variable *>(invocation->getObject().get());
    ASSERT_NE(nullptr, object);
    ASSERT_EQ("this", object->getName().t);

    MethodArguments args = invocation->getArgs();
    ASSERT_EQ(1, args.size());

    Variable * variable = dynamic_cast<Variable *>(args.front().get());
    ASSERT_NE(nullptr, variable);
    ASSERT_EQ("fst", variable->getName().t);
}
