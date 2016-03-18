#include "lexer/parsing_structures.h"

namespace fj {


    void AccessState::success(MethodInvocationState &methodInvocationState) {
        BaseMethodTermState::success(methodInvocationState);
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
        listOfArgsState.args.push_back(std::move(getTerm()));
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
        assert(nullptr == methodInvocation->getTerm());
        assert(listOfArgsState.args.size());
        methodInvocation->setTerm(std::move(listOfArgsState.args.back()));
        listOfArgsState.args.pop_back();
        listOfArgsState.args.push_back(std::move(getTerm()));
    }

    void AccessState::success(MethodState &methodState) {
        // If body already has a term it should belong to the
        // method invocation itself.
        if (methodState.methodDeclaration->getBodyTerm() != nullptr) {
            accessTerm->setTerm(methodState.methodDeclaration->getBodyTerm());
        }
        BaseMethodTermState::success(methodState);
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

    void ListOfArgsState::success(MethodInvocationState &methodInvocationState) {
        // TODO: Move vector itself?
        for (auto arg : args) {
            methodInvocationState.addArg(std::move(arg));
        }
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
