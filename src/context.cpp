#include "context.h"

bool Context::classHasProperty(ClassName className,
                               PropertyName propertyName) const {
    assert((bool)classes.count(className));
    ObjectClassBody *classBody = classes.find(className)->second;
    return (bool)classBody->getProperties().count(propertyName);
}

bool Context::isASubtype(std::string, std::string) const {
    return true;
}

Constructor *Context::invocateMethod(Constructor *object, MethodName methodName,
                                     MethodArguments args) {
    assert((bool)classes.count(object->getClassName()));
    ObjectClassBody *classBody = classes.find(object->getClassName())->second;
    MethodBody *methodBody = classBody->getMethod(methodName);
    return methodBody->invocate(object, args, this);
}

void Context::addClass(ObjectClassBody* body) {
    classes.insert(std::pair<ClassName, ObjectClassBody*>(body->getClassName(), body));
}

void Context::setVariables(map<PropertyName, Constructor *> vars) {
    this->assigned_variables = vars;

}

Constructor *Context::assignedValue(PropertyName name) const {
    assert((bool) assigned_variables.count(name));
    return assigned_variables.find(name)->second;
}

Context::~Context() {
    // TODO: Free assigned variables?
    for (auto elem : classes) {
//        std::cout << elem.first.t;
//        delete elem.second;
    }
}
