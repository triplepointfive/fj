#ifndef FJ_TERM_VISITOR_H
#define FJ_TERM_VISITOR_H

class Variable;

class Access;

class Invocation;

class Constructor;

class Coercion;

class TermVisitor {
public:
    virtual ~TermVisitor() { }

    virtual void visitVariable(Variable *) = 0;

    virtual void visitAccess(Access *) = 0;

    virtual void visitInvocation(Invocation *) = 0;

    virtual void visitConstructor(Constructor *) = 0;

    virtual void visitCoercion(Coercion *) = 0;
};

#endif //FJ_TERM_VISITOR_H
