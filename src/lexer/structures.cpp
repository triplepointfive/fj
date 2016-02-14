#include <lexer/lexemes.h>
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

    void PairState::success(ClassState & classState) {
        classState.classDeclaration->addProperty(key, val);
    }

    void MethodArgState::success(ConstructorState & constructorState) {
        constructorState.constructorBody->addArg(name, returnedClassName);
    }

    void MethodArgState::success(MethodState & methodState) {
        methodState.methodDeclaration->addArg(name, returnedClassName);
    }


    void BaseMethodTermState::success(MethodState & methodState) {
        methodState.methodDeclaration->setBodyTerm(
            std::move(getTerm())
        );
    }

    void BaseMethodTermState::success(MethodInvocationState & methodInvocationState) {
        methodInvocationState.addArg(std::move(getTerm()));
    }

    void BaseMethodTermState::success(TypeCastingState &typeCastingState) {
        typeCastingState.addArg(std::move(getTerm()));
    }

    void BaseMethodTermState::success(InitiationState &initiationState) {
        initiationState.addArg(std::move(getTerm()));
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
}
