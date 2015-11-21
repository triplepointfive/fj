#ifndef FJ_EVALTERMVISITOR_H
#define FJ_EVALTERMVISITOR_H

#include "term_visitor.h"

class EvalTermVisitor : public TermVisitor {
public:
    void visitVariable(Variable *) override;

    void visitAccess(Access *) override;

    void visitInvocation(Invocation *) override;

    void visitConstructor(Constructor *) override;

    void visitCoercion(Coercion *) override;
};

#endif //FJ_EVALTERMVISITOR_H
