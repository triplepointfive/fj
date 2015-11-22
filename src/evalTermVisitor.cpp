#include <assert.h>

#include "evalTermVisitor.h"
#include "context.h"

EvalTermVisitor::EvalTermVisitor() {
    calculatedValue = NULL;
}

void EvalTermVisitor::visitVariable(Variable *variable) {

}

void EvalTermVisitor::visitAccess(Access *access) {
    // E-Field
    access->getObject()->accept(*this);
    // The only accessible term is object.
    assert(calculatedValue != NULL);
    // E-ProjNew
    // This should be found by type checker.
    assert(context->classHasProperty(calculatedValue->getClassName(),
                                     access->getPropertyName()));
    calculatedValue->getAttribute(access->getPropertyName());
}

void EvalTermVisitor::visitInvocation(Invocation *invocation) {

}

void EvalTermVisitor::visitConstructor(Constructor *constructor) {
    calculatedValue = constructor;
}

void EvalTermVisitor::visitCoercion(Coercion *coercion) {

}
