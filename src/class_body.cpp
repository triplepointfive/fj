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