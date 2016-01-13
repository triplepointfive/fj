#ifndef FJ_EVALTERMVISITOR_H
#define FJ_EVALTERMVISITOR_H

#include "term_visitor.h"

class Context;

class EvalTermVisitor : public TermVisitor {
public:
    EvalTermVisitor(Context);

    void visitVariable(Variable *) override;

    void visitAccess(Access *) override;

    void visitInvocation(Invocation *) override;

    void visitConstructor(Constructor *) override;

    void visitCoercion(Coercion *) override;

    Constructor *getCalculatedValue() const { return calculatedValue; }

protected:
    Context *context;
    Constructor *calculatedValue;
};

#endif //FJ_EVALTERMVISITOR_H
