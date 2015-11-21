#include <assert.h>

#include "evalTermVisitor.h"
#include "term.h"
#include "context.h"

void EvalTermVisitor::visitVariable(Variable *variable) {

}

void EvalTermVisitor::visitAccess(Access *access) {
    // E-Field
    if (!access->getObject()->isValue()) {
        access->getObject()->accept(*this);
    }
    // E-ProjNew
    // This should be found by type checker.
    assert(context->classHasProperty("abd", access->getPropertyName()));
}

void EvalTermVisitor::visitInvocation(Invocation *invocation) {

}

void EvalTermVisitor::visitConstructor(Constructor *constructor) {

}

void EvalTermVisitor::visitCoercion(Coercion *coercion) {

}
