#include "lexer/parsing_structures.h"

namespace fj {

    void AccessState::success(MethodInvocationState &methodInvocationState) {
        std::cout << "5" << std::endl;
        BaseMethodTermState::success(methodInvocationState);
    }

    void AccessState::success(InitiationState &initiationState) {
        std::cout << "4" << std::endl;
        BaseMethodTermState::success(initiationState);
    }

    void AccessState::success(TypeCastingState &typeCastingState) {
        std::cout << "3" << std::endl;
        BaseMethodTermState::success(typeCastingState);
    }

    void AccessState::success(AccessState &accessState) {
        std::cout << "1" << std::endl;
        assert(nullptr == accessTerm->getTerm());
        assert(nullptr != accessState.accessTerm);
        accessTerm->setTerm(std::move(accessState.accessTerm));
        accessState.accessTerm = accessTerm;
    }

    void AccessState::success(ListOfArgsState &listOfArgsState) {
        std::cout << "2" << std::endl;
        assert(nullptr == accessTerm->getTerm());
        assert(listOfArgsState.args.size());
        accessTerm->setTerm(std::move(listOfArgsState.args.back()));
        listOfArgsState.args.pop_back();
        listOfArgsState.args.push_back(std::move(getTerm()));
    }

    void AccessState::success(MethodState &methodState) {
        std::cout << "6" << std::endl;
        // If body already has a term it should belong to the
        // access term itself.
        std::cout << getTerm()->inspect() << std::endl;
        if (methodState.methodDeclaration->getBodyTerm() != nullptr) {
            accessTerm->setTerm(methodState.methodDeclaration->getBodyTerm());
        }
        std::cout << getTerm()->inspect() << std::endl;
        BaseMethodTermState::success(methodState);
    }

    void MethodInvocationState::success(MethodInvocationState & methodInvocationState) {
        std::cout << "05" << std::endl;
        methodInvocationState.methodInvocation->setTerm(std::move(getTerm()));
    }

    void MethodInvocationState::success(TypeCastingState &typeCastingState) {
        std::cout << "06" << std::endl;
        typeCastingState.addArg(std::move(getTerm()));
    }

    void MethodInvocationState::success(AccessState &accessState) {
        std::cout << "07" << std::endl;
        accessState.addArg(std::move(getTerm()));
    }

    void MethodInvocationState::success(InitiationState &initiationState) {
        std::cout << "010" << std::endl;
        initiationState.addArg(std::move(getTerm()));
    }

    void MethodInvocationState::success(ListOfArgsState &listOfArgsState) {
        std::cout << "011" << std::endl;
        assert(nullptr == methodInvocation->getTerm());
        assert(listOfArgsState.args.size());
        methodInvocation->setTerm(std::move(listOfArgsState.args.back()));
        listOfArgsState.args.pop_back();
        listOfArgsState.args.push_back(std::move(getTerm()));
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
        std::cout << "012" << std::endl;
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
        std::cout << "77" << std::endl;
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
