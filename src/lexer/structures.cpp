#include "lexer/structures.h"

using namespace fj;

std::string MethodTerm::getName() const {
    return name;
}

void ConstructorBody::addProperty(const std::string &propertyName) {
    properties.push_back(propertyName);
}

void MethodDeclaration::setReturnClassName(const std::string &return_class_name) {
    this->return_class_name = return_class_name;
}

void MethodTermState::success(MethodInvocationState & methodInvocationState) {
    methodInvocationState.methodInvocation->addArg(std::move(methodTerm));
}
