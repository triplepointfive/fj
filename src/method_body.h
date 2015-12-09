#ifndef FJ_METHOD_BODY_H
#define FJ_METHOD_BODY_H

#include <map>

#include "term.h"

typedef std::map<PropertyName, ClassName> ArgumentsDeclaration;

class Term;
class Context;

class MethodBody {
public:
    MethodBody(MethodName, Term *, ArgumentsDeclaration);

    virtual ~MethodBody();

    Constructor *invocate(Constructor *object,
                          MethodArguments &arguments,
                          Context *context);

    MethodName getName() const { return name; }

protected:
    Term *body;
    MethodName name;
    ArgumentsDeclaration args;
};

#endif //FJ_METHOD_BODY_H