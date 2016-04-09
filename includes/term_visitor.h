#ifndef FJ_TERM_VISITOR_H
#define FJ_TERM_VISITOR_H

#include <memory>

#include "ast/term.h"

namespace fj {
    class TermVisitor {
    public:
        virtual ~TermVisitor() { }

        virtual void visitVariable(Variable *) = 0;

        virtual void visitAccess(Access *) = 0;

        virtual void visitInvocation(Invocation *) = 0;

        virtual void visitConstructor(Constructor *) = 0;

        virtual void visitCoercion(Coercion *) = 0;
    };

}

#endif //FJ_TERM_VISITOR_H
