#ifndef FJ_CONTEXT_H
#define FJ_CONTEXT_H

#include <string>
#include <map>

#include "ast/term.h"

namespace fj {
    class ClassTable;
    class ObjectClassBody;
    class Constructor;

    class Context {
    public:
        Context(std::shared_ptr< ClassTable >);
        void setVariables(std::map<PropertyName, std::shared_ptr< Constructor >>);

        std::shared_ptr< Constructor > invocateMethod(
            std::shared_ptr< Constructor > , MethodName, MethodArguments);

        std::shared_ptr< Constructor > assignedValue(PropertyName) const;

        TermPtr getAttribute(std::shared_ptr< Constructor >,
            PropertyName propertyName);

        std::shared_ptr< ClassTable > getClassTable() const;

    private:
        std::shared_ptr< ClassTable > classTable{ nullptr };
        std::map< PropertyName, std::shared_ptr< Constructor > > assigned_variables;
    };

}

#endif //FJ_CONTEXT_H
