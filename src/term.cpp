#include <iostream>

#include "term.h"
#include "term_visitor.h"

using namespace std;

string showArgs(Arguments args) {
    string acc = "";
    bool firstElement = true;

    for (auto &element : args) {
        if (firstElement) {
            firstElement = false;
        } else {
            acc += ", ";
        }
        acc += element->show();
    }
    return acc;
}

string Invocation::show() {
    return object->show() + "." + methodName + "(" + showArgs(args) + ")";
}

string Constructor::show() {
    return "new " + className + "(" + showArgs(args) + ")";
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
