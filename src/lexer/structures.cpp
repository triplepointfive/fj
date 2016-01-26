#include "lexer/structures.h"

namespace fj {
    std::string MethodTerm::getName() const {
        return name;
    }

    void ConstructorBody::addProperty(const std::string &propertyName) {
        properties.push_back(propertyName);
    }

    void ConstructorBody::addSuperArg(const std::string &variableName){
        superArgs.push_back(variableName);
    }

    void MethodDeclaration::setReturnClassName(const std::string &return_class_name) {
        this->return_class_name = return_class_name;
    }

    void MethodTermState::success(MethodInvocationState & methodInvocationState) {
        methodInvocationState.methodInvocation->addArg(std::move(methodTerm));
    }

    void PairState::success(ClassState & classState) {
        classState.classDeclaration->addProperty(key, val);
    }

    void MethodArgState::success(ConstructorState & constructorState) {
        constructorState.constructorBody->addArg(name, returnedClassName);
    }

    void MethodArgState::success(MethodState & methodState) {
        methodState.methodDeclaration->addArg(name, returnedClassName);
    }

    void MethodTermState::success(MethodState & methodState) {
        methodState.methodDeclaration->setBodyTerm(
            std::move(methodTerm)
        );
    }

    void MethodInvocationState::success(MethodTermState & methodTermState) {
        methodTermState.methodTerm = std::move(methodInvocation);
    }

    void MethodInvocationState::success(MethodInvocationState & methodInvocationState) {
        methodInvocationState.methodInvocation->addArg(std::move(methodInvocation));
    }

    void MethodState::success(ClassState & classState) {
        classState.classDeclaration->addMethod(
            std::move(methodDeclaration)
        );
    }

    void ConstructorState::success(ClassState & classState) {
        classState.classDeclaration->setConstructorBody(
            std::move(constructorBody)
        );
    }

    void ClassState::success(ParsedContext & context) {
        context.addClass(std::move(classDeclaration));
    }

    void MethodTermState::success(InitiationState &initiationState) {
        initiationState.initiation->addArg(std::move(methodTerm));
    }

    void MethodInvocationState::success(
        InitiationState &initiationState) {
        initiationState.initiation->addArg(std::move(methodInvocation));
    }

    void InitiationState::success(MethodState &methodState) {
        methodState.methodDeclaration->setBodyTerm(
            std::move(initiation)
        );
    }

    void InitiationState::success(
        MethodInvocationState &methodInvocationState) {
        methodInvocationState.methodInvocation->addArg(
            std::move(initiation)
        );
    }

    void InitiationState::success(InitiationState &initiationState) {
        initiationState.initiation->addArg(
            std::move(initiation)
        );
    }

    void InitiationState::success(MethodTermState &methodTermState) {
        // TODO: Check if pattern for MethodTermState is mandatory
        methodTermState.methodTerm = std::move(initiation);
    }
}
