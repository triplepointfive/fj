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
