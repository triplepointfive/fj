#include "class_body.h"

ClassBody::ClassBody(ClassName className, Properties properties,
                     Methods methods, ObjectClassBody *parentClass) {
    this->parentClass = parentClass;
    this->className = className;

    Properties parentProperties = parentClass->getProperties();
    this->properties = properties;
    this->properties.insert(parentProperties.begin(), parentProperties.end());

    this->methods = methods;
}

Properties ClassBody::getProperties() const {
    return properties;
}

MethodBody *ClassBody::getMethod(MethodName methodName) const {
    auto i = methods.find(methodName);
    if (methods.end() == i) {
        return i->second;
    } else {
        return parentClass->getMethod(methodName);
    }
}

ClassName ClassBody::getClassName() const {
    return className;
}

ClassBody::~ClassBody() {
    for (auto elem : methods) {
        delete elem.second;
    }
}
