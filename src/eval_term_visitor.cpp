#include <assert.h>

#include "eval_term_visitor.h"
#include "context.h"

EvalTermVisitor::EvalTermVisitor(Context ctx) {
    this->context = new Context(ctx);
    calculatedValue = nullptr;
}

void EvalTermVisitor::visitVariable(Variable *variable) {
    calculatedValue = context->assignedValue(variable->getName());
}

void EvalTermVisitor::visitAccess(Access *access) {
    // E-Field
    // TODO: Might be required to null before accessing.
    access->getObject()->accept(*this);
    // The only accessible term is object.
    assert(calculatedValue != NULL);
    // E-ProjNew
    // This should be found by type checker.
    assert(context->classHasProperty(calculatedValue->getClassName(),
                                     access->getPropertyName()));
    calculatedValue->getAttribute(access->getPropertyName())->accept(*this);
}

void EvalTermVisitor::visitInvocation(Invocation *invocation) {
    // E-Invk-Recv
    invocation->getObject()->accept(*this);
    // The only invocable term is object.
    assert(calculatedValue != NULL);
    // E-Invk-Arg
    MethodArguments calculatedArguments;
    for (auto elem : invocation->getArgs()) {
        EvalTermVisitor visitor(*this);
        elem.second->accept(visitor);
        calculatedArguments[elem.first] = visitor.getCalculatedValue();
    };
    // E-InvkNew
    calculatedValue = context->invocateMethod(calculatedValue,
                                              invocation->getMethodName(),
                                              calculatedArguments);
}

void EvalTermVisitor::visitConstructor(Constructor *constructor) {
    // E-New-Arg
    MethodArguments calculatedArguments;
    for (auto elem : constructor->getArgs()) {
        EvalTermVisitor visitor(*this);
        elem.second->accept(visitor);
        calculatedArguments[elem.first] = visitor.getCalculatedValue();
    };
    if (calculatedValue != nullptr) {
        delete calculatedValue;
    }
    calculatedValue = new Constructor(constructor->getClassName(),
                                      calculatedArguments);
}

void EvalTermVisitor::visitCoercion(Coercion *coercion) {
    // E-Cast
    // TODO: Might be required to null before casting.
    coercion->getObject()->accept(*this);
    // The only coercible term is object.
    assert(calculatedValue != NULL);
    // E-CastNew
    // No action for evaluation required, because type casting is about types.
    assert(context->isASubtype(coercion->getClassName(),
                               calculatedValue->getClassName()));
}
