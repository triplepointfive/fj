#ifndef FJ_PARSER_RULES_H
#define FJ_PARSER_RULES_H

#include <string>
#include <iostream>

#include <pegtl.hh>
#include <pegtl/contrib/changes.hh>
#include <ast/term.h>

#include "lexer/parsing_structures.h"
#include "lexer/errors.h"

namespace fj {
    using namespace pegtl;

    template< typename Rule >
    struct build_method_invocation : nothing< Rule > {};

    template<> struct build_method_invocation< method_name > {
        static void apply(const input & in,
                          MethodInvocationState & methodInvocationState) {
            methodInvocationState.methodInvocation->setName(in.string());
        }
    };

    template< typename Rule >
    struct build_instantiation : nothing< Rule > {};

    template<> struct build_instantiation< instantiation_class > {
        static void apply(const input & in,
                          InitiationState & initiationState) {
            initiationState.initiation->setName(in.string());
        }
    };

    template< typename Rule >
    struct build_casting : nothing< Rule > {};

    template<> struct build_casting< type_casting_class > {
        static void apply(const input & in,
                          TypeCastingState & typeCastingState) {
            typeCastingState.typeCastingTerm->setName(in.string());
        }
    };

    template< typename Rule > struct build_property_term : nothing< Rule > {};
    template<> struct build_property_term< attribute_name > {
        static void apply(const input & in, AccessState & accessState) {
            accessState.accessTerm->setName(in.string());
        }
    };

    template< typename Rule > struct build_variable_term : nothing< Rule > {};
    template<> struct build_variable_term< variable_term > {
        static void apply(const input & in, VariableTermState &variableTermState) {
            variableTermState.variableTerm->setName(in.string());
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

    template<> struct build_constructor< super_arg > {
        static void apply(const input & in, ConstructorState & constructorState) {
            constructorState.constructorBody->addSuperArg(in.string());
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

    template<> struct control< method_arg > :
        change_state_and_action< method_arg, MethodArgState, build_method_arg > {};

    // Method Terms
    template<> struct control< variable_term > :
        change_state_and_action< variable_term, VariableTermState, build_variable_term > {};
    template<> struct control< access_term > :
        change_state_and_action< access_term, AccessState, build_property_term > {};
    template<> struct control< method_invocation > :
        change_state_and_action< method_invocation, MethodInvocationState, build_method_invocation > {};
    template<> struct control< instantiation > :
        change_state_and_action< instantiation, InitiationState, build_instantiation > {};
    template<> struct control< type_casting > :
        change_state_and_action< type_casting, TypeCastingState, build_casting > {};

    template<> struct control< method_list_of_args > :
        change_state_and_action< method_list_of_args, ListOfArgsState, nothing > {};
}

#endif //FJ_PARSER_RULES_H
