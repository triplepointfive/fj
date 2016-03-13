#include "lexer/structures.h"
#include "lexer/lexemes.h"
#include "lexer/lexer_term_visitor.h"

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
        methodInvocationState.methodInvocation->setTerm(std::move(getTerm()));
    }

    void BaseMethodTermState::success(TypeCastingState &typeCastingState) {
        typeCastingState.addArg(std::move(getTerm()));
    }

    void BaseMethodTermState::success(AccessState &accessState) {
        accessState.addArg(std::move(getTerm()));
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

    void VariableTerm::accept(LexerTermVisitor* visitor) const {
        visitor->visitVariableTerm(this);
    }

    void AccessTerm::accept(LexerTermVisitor* visitor) const {
        visitor->visitAccessTerm(this);
    }

    void TypeCastingTerm::accept(LexerTermVisitor* visitor) const {
        visitor->visitTypeCastingTerm(this);
    }

    void MethodInvocation::accept(LexerTermVisitor* visitor) const {
        visitor->visitMethodInvocation(this);
    }

    void Initiation::accept(LexerTermVisitor* visitor) const {
        visitor->visitInitiation(this);
    }

    void ListOfArgsState::success(MethodInvocationState &methodInvocationState) {
        // TODO: Move vector itself?
        for (auto arg : args) {
            methodInvocationState.addArg(std::move(arg));
        }
    }

    void ListOfArgsState::success(InitiationState &initiationState) {
        // TODO: Move vector itself?
        for (auto arg : args) {
            initiationState.addArg(std::move(arg));
        }
    }

    void BaseMethodTermState::success(ListOfArgsState &listOfArgsState) {
        listOfArgsState.args.push_back(std::move(getTerm()));
    }

    void MethodInvocationState::success(MethodState &methodState) {
        // If body already has a term it should belong to the
        // method invocation itself.
        if (methodState.methodDeclaration->getBodyTerm() != nullptr) {
            methodInvocation->setTerm(std::move(
                methodState.methodDeclaration->getBodyTerm()
            ));
        }
        BaseMethodTermState::success(methodState);
    }

    void MethodInvocationState::success(MethodInvocationState & methodInvocationState) {
        methodInvocationState.methodInvocation->setTerm(std::move(getTerm()));
    }

    void MethodInvocationState::success(TypeCastingState &typeCastingState) {
        typeCastingState.addArg(std::move(getTerm()));
    }

    void MethodInvocationState::success(AccessState &accessState) {
        accessState.addArg(std::move(getTerm()));
    }

    void MethodInvocationState::success(InitiationState &initiationState) {
        initiationState.addArg(std::move(getTerm()));
    }

    void MethodInvocationState::success(ListOfArgsState &listOfArgsState) {
        listOfArgsState.args.push_back(std::move(getTerm()));
    }
}
