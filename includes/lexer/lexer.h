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

    template< typename Rule >
    struct build_method_body : nothing< Rule > {};

    template< typename Rule >
    struct build_method_invocation : nothing< Rule > {};

    template<> struct build_method_invocation< method_invocation_head > {
        static void apply(const input & in,
                          MethodInvocation **methodInvocation) {
//            std::cout << "build_method_invocation: method_invocation_head: " << in.string() << std::endl;
            std::string objectName, methodName;
            splitOnDot(in.string(), objectName, methodName);
            *methodInvocation = new MethodInvocation(objectName, methodName);
        }
    };

    template<> struct build_method_body< method_invocation > {
        static void apply( const input & in, MethodTerm **methodTerm) {
//            std::cout << "build_method_body: method_invocation: " << in.string() << std::endl;
            MethodInvocation *methodInvocation = nullptr;
            // TODO: Validate returned status.
            parse< fj::method_invocation, fj::build_method_invocation >(
                in.string(),
                "method_body rule",
                &methodInvocation
            );
            *methodTerm = methodInvocation;
        }
    };

    template<> struct build_method_body< property_invocation > {
        static void apply( const input & in, MethodTerm **methodTerm) {
//            std::cout << "build_method_body: property_term: " << in.string() << std::endl;
            std::string thisStr, propName;
            splitOnDot(in.string(), thisStr, propName);
            *methodTerm = new PropertyTerm(propName);
        }
    };

    template<> struct build_method_body< variable_term > {
        static void apply( const input & in, MethodTerm **methodTerm) {
//            std::cout << "build_method_body: variable_term: " << in.string() << std::endl;
            *methodTerm = new VariableTerm(in.string());
        }
    };

    template< typename Rule >
    struct build_constructor : nothing< Rule > {};

    template<> struct build_constructor< constructor_name > {
        static void apply(const input & in, ConstructorBody & constructorBody) {
            constructorBody.setName(in.string());
        }
    };

    template<> struct build_constructor< assignment_prop_name > {
        static void apply(const input & in, ConstructorBody & constructorBody) {
            constructorBody.addProperty(in.string());
        }
    };

    template< typename Rule >
    struct build_method : nothing< Rule > {};

    template<> struct build_method< method_ret_and_name > {
        static void apply(const input & in, MethodDeclaration & method) {
//            std::cout << "method_ret_and_name: " << in.string() << std::endl;
            std::string className, methodName;
            splitOnSpace(in.string(), className, methodName);
            method.setName(methodName);
            method.setReturnClassName(className);
        }
    };

    // Assigns top node of body tree to the current method declaration.
    template<> struct build_method< method_body > {
        static void apply(const input & in, MethodDeclaration & method) {
            MethodTerm *methodTerm = nullptr;
            // TODO: Validate returned status.
            parse< fj::method_body, fj::build_method_body >(
                in.string(),
                "method_body rule",
                &methodTerm
            );
            method.setBodyTerm(methodTerm);
        }
    };

    // Builds a pair representing a property.
    template< typename Rule >
    struct build_method_arg : nothing< Rule > {};

    template<> struct build_method_arg< class_name > {
        static void apply(const input & in, MethodArg & methodArg) {
            methodArg.returnedClassName = in.string();
        }
    };

    template<> struct build_method_arg< object_name > {
        static void apply(const input & in, MethodArg & methodArg) {
            methodArg.name = in.string();
        }
    };

    // Builds a pair representing a property.
    template< typename Rule >
    struct build_class_property : nothing< Rule > {};

    template<> struct build_class_property< class_name > {
        static void apply(const input & in, Pair & pair) {
            pair.key = in.string();
        }
    };

    template<> struct build_class_property< object_name > {
        static void apply(const input & in, Pair & pair) {
            pair.val = in.string();
        }
    };

    // Action to build a class.
    template< typename Rule >
    struct build_class : nothing< Rule > {};

    template<> struct build_class< declared_class_name > {
        static void apply(const input & in, ClassDeclaration & classDeclaration) {
            classDeclaration.setName(in.string());
        }
    };

    template<> struct build_class< inherited_class_name > {
        static void apply(const input & in, ClassDeclaration & classDeclaration) {
            classDeclaration.setParentName(in.string());
        }
    };

    // Top control switcher.
    template< typename Rule > struct control : errors< Rule > {};
    template<> struct control< class_def > :
        change_state_and_action< class_def, ClassDeclaration, build_class > {};
    template<> struct control< property_def > :
        change_state_and_action< property_def, Pair, build_class_property > {};
    template<> struct control< method_arg > :
        change_state_and_action< method_arg, MethodArg, build_method_arg > {};
}

#endif //FJ_AST_STRUCT_H
