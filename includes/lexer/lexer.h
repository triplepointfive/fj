#ifndef FJ_AST_STRUCT_H
#define FJ_AST_STRUCT_H

#include <string>
#include <iostream>

#include <pegtl.hh>
#include <pegtl/contrib/changes.hh>

#include "lexer/structures.h"
#include "lexer/errors.h"

namespace fj {
    using namespace pegtl;

    struct ClassState {
        std::shared_ptr< ClassDeclaration > classDeclaration =
            std::make_shared< ClassDeclaration >();

        void success(ParsedContext & context) {
            context.addClass(std::move(classDeclaration));
        }
    };

    struct ConstructorState {
        std::shared_ptr< ConstructorBody > constructorBody =
            std::make_shared< ConstructorBody >();

        void success(ClassState & classState) {
            classState.classDeclaration->setConstructorBody(
                std::move(constructorBody)
            );
        }
    };

    struct MethodState {
        std::shared_ptr< MethodDeclaration > methodDeclaration =
            std::make_shared< MethodDeclaration >();

        void success(ClassState & classState) {
            classState.classDeclaration->addMethod(
                std::move(methodDeclaration)
            );
        }
    };

    struct MethodTermState {
        std::shared_ptr< MethodTerm > methodTerm = nullptr;

        void success(MethodState & methodState) {
            methodState.methodDeclaration->setBodyTerm(
                std::move(methodTerm)
            );
        }
    };

    struct PairState {
        std::string key;
        std::string val;

        void success(ClassState & classState) {
            classState.classDeclaration->addProperty(key, val);
        }
    };

    struct MethodArgState {
        std::string name;
        std::string returnedClassName;

        void success(ConstructorState & constructorState) {
            constructorState.constructorBody->addArg(name, returnedClassName);
        }

        void success(MethodState & methodState) {
            methodState.methodDeclaration->addArg(name, returnedClassName);
        }
    };

    template< typename Rule >
    struct build_method_body : nothing< Rule > {};

    template< typename Rule >
    struct build_method_invocation : nothing< Rule > {};

    template<> struct build_method_invocation< method_invocation_head > {
        static void apply(const input & in,
                          MethodInvocation **methodInvocation) {
            std::string objectName, methodName;
            splitOnDot(in.string(), objectName, methodName);
            *methodInvocation = new MethodInvocation(objectName, methodName);
        }
    };

    template<> struct build_method_body< method_invocation > {
        static void apply(const input & in, MethodTermState & methodTermState) {
            MethodInvocation *methodInvocation = nullptr;
            // TODO: Validate returned status.
            parse< fj::method_invocation, fj::build_method_invocation >(
                in.string(),
                "method_invocation rule",
                &methodInvocation
            );
            // TODO: Validate assignment
            methodTermState.methodTerm =
                std::make_shared< MethodInvocation >(*methodInvocation);
        }
    };

    template<> struct build_method_body< property_invocation > {
        static void apply(const input & in, MethodTermState & methodTermState) {
            std::string thisStr, propName;
            splitOnDot(in.string(), thisStr, propName);
            methodTermState.methodTerm =
                std::make_shared < PropertyTerm >(propName);
        }
    };

    template<> struct build_method_body< variable_term > {
        static void apply(const input & in, MethodTermState & methodTermState) {
            methodTermState.methodTerm =
                std::make_shared < VariableTerm >(in.string());
        }
    };

    template< typename Rule >
    struct build_constructor : nothing< Rule > {};

    template<> struct build_constructor< constructor_name > {
        static void apply(const input & in, ConstructorState & constructorState) {
            constructorState.constructorBody->setName(in.string());
        }
    };

    template<> struct build_constructor< assignment_prop_name > {
        static void apply(const input & in, ConstructorState & constructorState) {
            constructorState.constructorBody->addProperty(in.string());
        }
    };

    template< typename Rule >
    struct build_method : nothing< Rule > {};

    template<> struct build_method< met_class > {
        static void apply(const input & in, MethodState & methodState) {
            methodState.methodDeclaration->setReturnClassName(in.string());
        }
    };

    template<> struct build_method< met_name > {
        static void apply(const input & in, MethodState & methodState) {
            methodState.methodDeclaration->setName(in.string());
        }
    };

    // Builds a pair representing a property.
    template< typename Rule >
    struct build_method_arg : nothing< Rule > {};

    template<> struct build_method_arg< class_name > {
        static void apply(const input & in, MethodArgState & methodArg) {
            methodArg.returnedClassName = in.string();
        }
    };

    template<> struct build_method_arg< object_name > {
        static void apply(const input & in, MethodArgState & methodArg) {
            methodArg.name = in.string();
        }
    };

    // Builds a pair representing a property.
    template< typename Rule >
    struct build_class_property : nothing< Rule > {};

    template<> struct build_class_property< class_name > {
        static void apply(const input & in, PairState & pair) {
            pair.key = in.string();
        }
    };

    template<> struct build_class_property< object_name > {
        static void apply(const input & in, PairState & pair) {
            pair.val = in.string();
        }
    };

    // Action to build a class.
    template< typename Rule >
    struct build_class : nothing< Rule > {};

    template<> struct build_class< declared_class_name > {
        static void apply(const input & in, ClassState & classState) {
            classState.classDeclaration->setName(in.string());
        }
    };

    template<> struct build_class< inherited_class_name > {
        static void apply(const input & in, ClassState & classState) {
            classState.classDeclaration->setParentName(in.string());
        }
    };

    // Top control switcher.
    template< typename Rule > struct control : errors< Rule > {};
    template<> struct control< class_def > :
        change_state_and_action< class_def, ClassState, build_class > {};
    template<> struct control< property_def > :
        change_state_and_action< property_def, PairState, build_class_property > {};
    template<> struct control< constructor_def > :
        change_state_and_action< constructor_def, ConstructorState, build_constructor > {};
    template<> struct control< method_def > :
        change_state_and_action< method_def, MethodState, build_method > {};
    template<> struct control< method_body > :
        change_state_and_action< method_body, MethodTermState, build_method_body > {};
    template<> struct control< method_arg > :
        change_state_and_action< method_arg, MethodArgState, build_method_arg > {};
}

#endif //FJ_AST_STRUCT_H
