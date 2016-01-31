#include "ast/class_body.h"

ClassBody::ClassBody(const ClassName &className, Properties properties,
                     Methods methods, std::shared_ptr< ObjectClassBody > parentClass) :
    className(className),
    parentClass(parentClass) {
    Properties parentProperties = parentClass->getProperties();
    this->properties = properties;
    this->properties.insert(parentProperties.begin(), parentProperties.end());

    this->methods = methods;
}

Properties ClassBody::getProperties() const {
    return properties;
}

std::shared_ptr< MethodBody > ClassBody::getMethod(
    MethodName methodName) const {
    auto i = methods.find(methodName);
    if (methods.end() == i) {
        return parentClass->getMethod(methodName);
    } else {
        return std::move(i->second);
    }
}

ClassName ClassBody::getClassName() const {
    return className;
}
