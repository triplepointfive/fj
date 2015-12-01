#include "class_body.h"

ClassBody::ClassBody(ClassName className, Properties properties,
                     Methods methods, ObjectClassBody *parentClass) {
    this->parentClass = parentClass;
    this->className = className;

    Properties parentProperties = parentClass->getProperties();
    this->properties = properties;
    this->properties.insert(parentProperties.begin(), parentProperties.end());

    Methods parentMethods = parentClass->getMethods();
    this->methods = methods;
    this->methods.insert(parentMethods.begin(), parentMethods.end());
}

Properties ClassBody::getProperties() const {
    return properties;
}

Methods ClassBody::getMethods() const {
    return methods;
}
