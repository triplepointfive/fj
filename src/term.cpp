#include <iostream>

#include "term.h"
#include "term_visitor.h"
#include "utils.h"

using namespace std;

string Invocation::show() const {
    return object->show() + "." + methodName.t +
           "(" + intercalation(args, ", ") + ")";
}

string Constructor::show() const {
    return "new " + className.t + "(" + intercalation(args, ", ") + ")";
}

void Variable::accept(TermVisitor &visitor) {
    visitor.visitVariable(this);
}

void Access::accept(TermVisitor &visitor) {
    visitor.visitAccess(this);
}

void Invocation::accept(TermVisitor &visitor) {
    visitor.visitInvocation(this);
}

void Constructor::accept(TermVisitor &visitor) {
    visitor.visitConstructor(this);
}

void Coercion::accept(TermVisitor &visitor) {
    visitor.visitCoercion(this);
}
