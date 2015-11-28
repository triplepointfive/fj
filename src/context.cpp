#include "context.h"

bool Context::classHasProperty(ClassName className,
                               PropertyName propertyName) const {
    assert((bool)classes.count(className));
    ClassBody classBody = classes.find(className)->second;
    return (bool)classBody.getProperties().count(propertyName);
}

bool Context::isASubtype(std::string parent, std::string child) const {
    return true;
}

Constructor *Context::invocateMethod(Constructor *object, string methodName,
                                     Arguments args) const {
    return nullptr;
}

void Context::addClass(ClassName name, ClassBody body) {
    classes.insert ( std::pair<ClassName, ClassBody>(name, body) );
}
