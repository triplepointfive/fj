#ifndef FJ_EVALTERMVISITOR_H
#define FJ_EVALTERMVISITOR_H

#include "term_visitor.h"

namespace fj {

    class Context;

    class EvalTermVisitor : public TermVisitor {
    public:
        EvalTermVisitor(Context);

        void visitVariable(Variable *) override;

        void visitAccess(Access *) override;

        void visitInvocation(Invocation *) override;

        void visitConstructor(Constructor *) override;

        void visitCoercion(Coercion *) override;

        std::shared_ptr< Constructor > getCalculatedValue() const {
            return calculatedValue;
        }

    protected:
        Context *context;
        std::shared_ptr< Constructor > calculatedValue;
    };
    
}

#endif //FJ_EVALTERMVISITOR_H
