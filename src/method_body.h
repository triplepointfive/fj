#ifndef FJ_METHOD_BODY_H
#define FJ_METHOD_BODY_H

#include <map>

#include "term.h"

typedef std::map<std::string, std::string> ArgumentsDeclaration;

class Term;

class MethodBody {
public:
    MethodBody(Term *, ArgumentsDeclaration);

    virtual ~MethodBody();

    Constructor *invocate(Constructor *, Arguments);

protected:
    Term *body;
    ArgumentsDeclaration args;
};


#endif //FJ_METHOD_BODY_H
