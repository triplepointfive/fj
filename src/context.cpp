#include "context.h"

namespace fj {
    using std::map;

    bool Context::classHasProperty(ClassName className,
                                   PropertyName propertyName) const {
        assert((bool)classes.count(className));
        auto classBody = classes.find(className)->second;
        return (bool)classBody->getProperties().count(propertyName);
    }

    bool Context::isASubtype(std::string, std::string) const {
        return true;
    }

    std::map<ClassName, std::shared_ptr< ObjectClassBody >>
        Context::getClasses() const {
        return std::move(classes);
    }


    TermPtr Context::getAttribute(std::shared_ptr< Constructor > object,
        PropertyName propertyName) {
        assert((bool)classes.count(object->getClassName()));
        auto classBody = classes.find(object->getClassName())->second;
        return classBody->getProperty(object, propertyName);
    }

    std::shared_ptr< Constructor > Context::invocateMethod(
        std::shared_ptr< Constructor > object, MethodName methodName,
        MethodArguments args) {
        assert((bool)classes.count(object->getClassName()));
        auto classBody = classes.find(object->getClassName())->second;
        auto methodBody = classBody->getMethod(methodName);
        return methodBody->invocate(object, args, this);
    }

    void Context::addClass(std::shared_ptr< ObjectClassBody > body) {
        classes.insert(std::pair<ClassName, std::shared_ptr< ObjectClassBody >>(
            body->getClassName(), body
        ));
    }

    void Context::setVariables(
        map<PropertyName, std::shared_ptr< Constructor >> vars) {
        this->assigned_variables = vars;

    }

    std::shared_ptr< Constructor > Context::assignedValue(PropertyName name) const {
        assert((bool) assigned_variables.count(name));
        return assigned_variables.find(name)->second;
    }
}
