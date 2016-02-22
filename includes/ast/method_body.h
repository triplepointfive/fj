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

        std::shared_ptr< Constructor > invocate(
            std::shared_ptr< Constructor > object,
            MethodArguments &arguments,
            Context *context);

        MethodName getName() const { return name; }

    protected:
        MethodName name;
        std::shared_ptr< Term > body;
        ArgumentsDeclaration args;
    };

}

#endif //FJ_METHOD_BODY_H
