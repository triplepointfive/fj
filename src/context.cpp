#include "context.h"

bool Context::classHasProperty(std::string className,
                               std::string propertyName) const {
    return false;
}

bool Context::isASubtype(std::string parent, std::string child) const {
    return true;
}

Constructor *Context::invocateMethod(Constructor *object, string methodName,
                                     Arguments args) const {
    return nullptr;
}
