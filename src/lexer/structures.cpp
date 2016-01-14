#include "lexer/structures.h"

std::string MethodTerm::getName() const {
    return name;
}

void ConstructorBody::addProperty(const std::string &propertyName) {
    properties.push_back(propertyName);
}

void MethodDeclaration::setReturnClassName(const std::string &return_class_name) {
    this->return_class_name = return_class_name;
}

void ClassDeclaration::success(ParsedContext & context) {
    context.addClass(*this);
}

void MethodArg::success(ConstructorBody & constructorBody) {
    constructorBody.addArg(name, returnedClassName);
}

void MethodArg::success(MethodDeclaration & methodDeclaration) {
    methodDeclaration.addArg(name, returnedClassName);
}

void Pair::success(ClassDeclaration & classDeclaration) {
    classDeclaration.addProperty(key, val);
}