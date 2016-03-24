#include "lexer/parsing_structures.h"

namespace fj {

    void AccessState::success(MethodInvocationState &methodInvocationState) {
        assert(nullptr != methodInvocationState.getTerm());
        assert(nullptr == accessTerm->getTerm());
        accessTerm->setTerm(methodInvocationState.getTerm());
        methodInvocationState.methodInvocation = std::move(accessTerm);
    }

    void AccessState::success(InitiationState &initiationState) {
        BaseMethodTermState::success(initiationState);
    }

    void AccessState::success(TypeCastingState &typeCastingState) {
        BaseMethodTermState::success(typeCastingState);
    }

    void AccessState::success(AccessState &accessState) {
        assert(nullptr == accessTerm->getTerm());
        assert(nullptr != accessState.accessTerm);
        accessTerm->setTerm(std::move(accessState.accessTerm));
        accessState.accessTerm = accessTerm;
    }

    void AccessState::success(ListOfArgsState &listOfArgsState) {
        assert(nullptr == accessTerm->getTerm());
        assert(listOfArgsState.args.size());
        accessTerm->setTerm(std::move(listOfArgsState.args.back()));
        listOfArgsState.args.pop_back();
        listOfArgsState.args.push_back(getTerm());
    }

    void AccessState::success(MethodState &methodState) {
        // If body already has a term it should belong to the
        // access term itself.
        if (methodState.methodDeclaration->getBodyTerm() != nullptr) {
            accessTerm->setTerm(methodState.methodDeclaration->getBodyTerm());
        }
        BaseMethodTermState::success(methodState);
    }

    void MethodInvocationState::success(MethodInvocationState & methodInvocationState) {
        assert(nullptr == methodInvocation->getTerm());
        assert(nullptr != methodInvocationState.methodInvocation);
        methodInvocation->setTerm(std::move(methodInvocationState.methodInvocation));
        methodInvocationState.methodInvocation = methodInvocation;
    }

    void MethodInvocationState::success(TypeCastingState &typeCastingState) {
        typeCastingState.addArg(getTerm());
    }

    void MethodInvocationState::success(AccessState &accessState) {
        assert(nullptr != accessState.getTerm());
        assert(nullptr == methodInvocation->getTerm());
        methodInvocation->setTerm(accessState.getTerm());
        accessState.accessTerm = std::move(methodInvocation);
    }

    void MethodInvocationState::success(InitiationState &initiationState) {
        initiationState.addArg(getTerm());
    }

    void MethodInvocationState::success(ListOfArgsState &listOfArgsState) {
        assert(nullptr == methodInvocation->getTerm());
        assert(listOfArgsState.args.size());
        methodInvocation->setTerm(std::move(listOfArgsState.args.back()));
        listOfArgsState.args.pop_back();
        listOfArgsState.args.push_back(getTerm());
    }

    void ListOfArgsState::success(InitiationState &initiationState) {
        initiationState.initiation->setArgs(args);
    }

    void ListOfArgsState::success(MethodInvocationState &methodInvocationState) {
        // TODO: Move vector itself?
        for (auto arg : args) {
            methodInvocationState.addArg(std::move(arg));
        }
    }

    void BaseMethodTermState::success(ListOfArgsState &listOfArgsState) {
        listOfArgsState.args.push_back(getTerm());
    }

    void MethodInvocationState::success(MethodState &methodState) {
        // If body already has a term it should belong to the
        // method invocation itself.
        if (methodState.methodDeclaration->getBodyTerm() != nullptr) {
            methodInvocation->setTerm(
                methodState.methodDeclaration->getBodyTerm()
            );
        }
        BaseMethodTermState::success(methodState);
    }

    void MethodArgState::success(MethodState & methodState) {
        methodState.methodDeclaration->addArg(name, returnedClassName);
    }

    void BaseMethodTermState::success(MethodState & methodState) {
        methodState.methodDeclaration->setBodyTerm(getTerm());
    }

    void BaseMethodTermState::success(MethodInvocationState & methodInvocationState) {
        methodInvocationState.methodInvocation->setTerm(getTerm());
    }

    void BaseMethodTermState::success(TypeCastingState &typeCastingState) {
        typeCastingState.addArg(getTerm());
    }

    void BaseMethodTermState::success(AccessState &accessState) {
        accessState.addArg(getTerm());
    }

    void BaseMethodTermState::success(InitiationState &initiationState) {
        initiationState.addArg(getTerm());
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

    void PairState::success(ClassState & classState) {
        classState.classDeclaration->addProperty(key, val);
    }

    void MethodArgState::success(ConstructorState & constructorState) {
        constructorState.constructorBody->addArg(name, returnedClassName);
    }

    void ClassState::success(ParsedContext & context) {
        context.addClass(std::move(classDeclaration));
    }
}
