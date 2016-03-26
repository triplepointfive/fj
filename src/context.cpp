#include "context.h"
#include "ast/class_table.h"

namespace fj {
    using std::map;

    Context::Context(std::shared_ptr< ClassTable > classTable) :
        classTable(classTable) {}

    std::shared_ptr< ClassTable > Context::getClassTable() const {
        return std::move(classTable);
    }

    TermPtr Context::getAttribute(std::shared_ptr< Constructor > object,
        PropertyName propertyName) {
        auto classBody = classTable->getClass(object->getClassName());
        return classBody->getProperty(object, propertyName);
    }

    std::shared_ptr< Constructor > Context::invocateMethod(
        std::shared_ptr< Constructor > object, MethodName methodName,
        MethodArguments args) {
        auto classBody = classTable->getClass(object->getClassName());
        auto methodBody = classBody->getMethod(methodName);
        return methodBody->invocate(object, args, this);
    }

    void Context::setVariables(
        map<PropertyName, std::shared_ptr< Constructor >> vars) {
        this->assigned_variables = vars;
    }

    std::shared_ptr< Constructor > Context::assignedValue(
            PropertyName name) const {
        assert((bool) assigned_variables.count(name));
        return assigned_variables.find(name)->second;
    }
}
