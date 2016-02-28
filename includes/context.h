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

        void setVariables(std::map<PropertyName, std::shared_ptr< Constructor >>);

        bool classHasProperty(ClassName className, PropertyName propertyName) const;

        bool isASubtype(std::string, std::string) const;

        std::shared_ptr< Constructor > invocateMethod(
            std::shared_ptr< Constructor > , MethodName, MethodArguments);

        std::shared_ptr< Constructor > assignedValue(PropertyName) const;

        TermPtr getAttribute(std::shared_ptr< Constructor >,
            PropertyName propertyName);

    private:
        std::map<ClassName, std::shared_ptr< ObjectClassBody >> classes;
        std::map< PropertyName, std::shared_ptr< Constructor > > assigned_variables;
    };

}

#endif //FJ_CONTEXT_H
