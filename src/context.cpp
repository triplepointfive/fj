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
                                     MethodArguments args) const {
    assert((bool)classes.count(object->getClassName()));
    ObjectClassBody *classBody = classes.find(object->getClassName())->second;
    assert((bool)classBody->getMethods().count(methodName));
    MethodBody *methodBody = classBody->getMethods().find(methodName)->second;
    return methodBody->invocate(object, args, nullptr);
}

void Context::addClass(ClassName name, ObjectClassBody* body) {
    classes.insert(std::pair<ClassName, ObjectClassBody*>(name, body));
}
