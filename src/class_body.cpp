#include "class_body.h"

ClassBody::ClassBody(ClassName className, Properties properties,
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

MethodBody *ClassBody::getMethod(MethodName methodName) const {
    auto i = methods.find(methodName);
    if (methods.end() == i) {
        return parentClass->getMethod(methodName);
    } else {
        return i->second;
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
