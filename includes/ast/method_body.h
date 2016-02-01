#ifndef FJ_METHOD_BODY_H
#define FJ_METHOD_BODY_H

#include <map>
#include <memory>

#include "term.h"

namespace fj {

    typedef std::map<PropertyName, ClassName> ArgumentsDeclaration;

    class Term;
    class Context;

    class MethodBody {
    public:
        MethodBody(const MethodName&, std::shared_ptr< Term >, ArgumentsDeclaration);

        Constructor *invocate(Constructor *object,
                              MethodArguments &arguments,
                              Context *context);

        MethodName getName() const { return name; }

    protected:
        std::shared_ptr< Term > body;
        MethodName name;
        ArgumentsDeclaration args;
    };

}

#endif //FJ_METHOD_BODY_H
