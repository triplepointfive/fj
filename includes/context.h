#ifndef FJ_CONTEXT_H
#define FJ_CONTEXT_H

#include <string>
#include <map>

#include "ast/term.h"
#include "ast/class_body.h"

namespace fj {

    class Context {
    public:
        void addClass(std::shared_ptr< ObjectClassBody >);

        void setVariables(map<PropertyName, Constructor *>);

        bool classHasProperty(ClassName className, PropertyName propertyName) const;

        bool isASubtype(std::string, std::string) const;

        Constructor *invocateMethod(Constructor *, MethodName, MethodArguments);

        Constructor *assignedValue(PropertyName) const;

    private:
        map<ClassName, std::shared_ptr< ObjectClassBody >> classes;
        map<PropertyName, Constructor *> assigned_variables;
    };

}

#endif //FJ_CONTEXT_H
