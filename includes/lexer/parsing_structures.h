#ifndef FJ_PARSING_STRUCTURES_H
#define FJ_PARSING_STRUCTURES_H

#include "lexer/structures.h"

namespace fj {
    struct ClassState {
        std::shared_ptr< ClassDeclaration > classDeclaration =
            std::make_shared< ClassDeclaration >();

        void success(ParsedContext & context);
    };

    struct ConstructorState {
        std::shared_ptr< ConstructorBody > constructorBody =
            std::make_shared< ConstructorBody >();

        void success(ClassState & classState);
    };

    struct MethodState {
        std::shared_ptr< MethodDeclaration > methodDeclaration =
            std::make_shared< MethodDeclaration >();

        void success(ClassState & classState);
    };

    struct MethodInvocationState;
    struct InitiationState;
    struct TypeCastingState;
    struct AccessState;
    struct ListOfArgsState;

    struct BaseMethodTermState {
        virtual std::shared_ptr< MethodTerm > getTerm() = 0;

        virtual void success(MethodState & methodState);
        virtual void success(MethodInvocationState & methodInvocationState);
        virtual void success(InitiationState & initiationState);
        virtual void success(TypeCastingState & typeCastingState);
        virtual void success(AccessState & accessState);
        virtual void success(ListOfArgsState & listOfArgsState);

        virtual ~BaseMethodTermState() {}
    };

    struct VariableTermState : BaseMethodTermState {
        std::shared_ptr< VariableTerm > variableTerm =
            std::make_shared< VariableTerm >();

        void addArg(std::shared_ptr< MethodTerm >) {
            // Shouldn't get called.
            throw "xxx";
        }

        std::shared_ptr< MethodTerm > getTerm() override {
            return variableTerm;
        };
    };

    struct ArguableTermState : BaseMethodTermState {
        virtual void addArg(std::shared_ptr< MethodTerm >) = 0;
        virtual ~ArguableTermState() {}
    };

    struct InitiationState : ArguableTermState {
        std::shared_ptr< Initiation > initiation =
            std::make_shared<Initiation>();

        void addArg(std::shared_ptr< MethodTerm > term) {
            initiation->addArg(term);
        }

        std::shared_ptr< MethodTerm > getTerm() override {
            return initiation;
        };
    };

    struct MethodInvocationState : ArguableTermState {
        std::shared_ptr< MethodInvocation > methodInvocation =
            std::make_shared< MethodInvocation >();

        void addArg(std::shared_ptr< MethodTerm > term) {
            methodInvocation->addArg(term);
        }

        std::shared_ptr< MethodTerm > getTerm() override {
            return methodInvocation;
        };

        void success(MethodState & methodState) override;
        void success(MethodInvocationState & methodInvocationState) override;
        void success(InitiationState & initiationState) override;
        void success(TypeCastingState & typeCastingState) override;
        void success(AccessState & accessState) override;
        void success(ListOfArgsState & listOfArgsState) override;
    };

    struct TypeCastingState : ArguableTermState {
        std::shared_ptr< TypeCastingTerm > typeCastingTerm =
            std::make_shared< TypeCastingTerm >();

        void addArg(std::shared_ptr< MethodTerm > term) {
            typeCastingTerm->setTerm(term);
        }

        std::shared_ptr< MethodTerm > getTerm() override {
            return typeCastingTerm;
        };
    };

    struct AccessState : ArguableTermState {
        std::shared_ptr<AccessTerm> accessTerm =
            std::make_shared<AccessTerm>();

        void addArg(std::shared_ptr< MethodTerm > term) {
            accessTerm->setTerm(term);
        }

        std::shared_ptr< MethodTerm > getTerm() override {
            return accessTerm;
        };

        void success(MethodState & methodState) override;
        void success(MethodInvocationState & methodInvocationState) override;
        void success(InitiationState & initiationState) override;
        void success(TypeCastingState & typeCastingState) override;
        void success(AccessState & accessState) override;
        void success(ListOfArgsState & listOfArgsState) override;
    };

    struct ListOfArgsState {
        std::vector< std::shared_ptr< MethodTerm > > args;
        void success(MethodInvocationState & methodInvocationState);
        void success(InitiationState & initiationState);
    };

    struct PairState {
        std::string key;
        std::string val;

        void success(ClassState & classState);
    };

    struct MethodArgState {
        std::string name;
        std::string returnedClassName;

        void success(ConstructorState & constructorState);
        void success(MethodState & methodState);
    };
}

#endif //FJ_PARSING_STRUCTURES_H
