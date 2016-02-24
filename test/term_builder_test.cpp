#include <gtest/gtest.h>
#include "ast/builder.h"

using namespace fj;

TEST (term_builder, variable_term) {
    TermPtr term = { nullptr };
    std::shared_ptr< VariableTerm > variableTerm
        = std::make_shared< VariableTerm >("total");

    TermBuilder::build(term, variableTerm);

    ASSERT_NE(nullptr, term);
    Variable * variable = dynamic_cast<Variable *>(term.get());
    ASSERT_NE(nullptr, variable);
    ASSERT_EQ("total", variable->getName().t);
}
