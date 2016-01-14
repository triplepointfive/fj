#ifndef FJ_LEXEMES_H
#define FJ_LEXEMES_H

#include <pegtl.hh>

namespace fj {
    using namespace pegtl;

    /* Keywords */
    struct class_keyword : pegtl_string_t( "class" ) {};
    struct extends_keyword : pegtl_string_t( "extends" ) {};
    struct super_keyword : pegtl_string_t( "super" ) {};
    struct return_keyword : pegtl_string_t( "return" ) {};
    struct new_keyword : pegtl_string_t( "new" ) {};
    struct this_keyword : pegtl_string_t( "this" ) {};

    /* Special characters with optional paddies */
    struct semicolon : pad < one < ';' >, space, space > {};
    struct open_brace : pad < one < '{' >, space, space > {};
    struct close_brace : pad < one < '}' >, space, space > {};
    struct open_bracket : pad < one < '(' >, space, space > {};
    struct close_bracket : pad < one < ')' >, space, space > {};
    struct comma : pad < one < ',' >, space, space > {};
    struct assign : pad < one < '=' >, space, space > {};

    /* Grammar */
    struct class_name : plus< identifier > {};

    struct method_name : plus< identifier > {};

    // Could be used for both property and argument names.
    struct object_name : plus< identifier > {};

    // Matches the name of new class.
    struct declared_class_name : class_name {};

    // Matches the class name after the extends keyword.
    struct inherited_class_name : class_name {};

    // Matches new property inside class body.
    struct property_def : seq < class_name, space, object_name > {};

    // A list of base units inside class body: properties, methods etc.
    struct class_terms : sor < property_def > {};

    // Ignores mandatory leading and trailing spaces.
    template < typename Rule >
    struct lexeme : seq < space, Rule, space> {};

    // Matches the pattern like "{Rule}"
    template < typename Rule >
    struct sur_with_braces : seq < open_brace, Rule, close_brace > {};

    // Matches the pattern like "(Rule)"
    template < typename Rule >
    struct sur_with_brackets : seq < open_bracket, Rule, close_bracket > {};

    /* Terms */

    struct method_term; // Forwarding declaration for cycle usage.

    // Matches invocation of super.
    struct super_invocation :
            seq < super_keyword, sur_with_brackets < success > > {};

    // Just a variable name, matching general restrictions.
    struct variable_term : object_name {};

    // Left part of assignment, matches "this.fst".
    struct property_invocation :
            seq < this_keyword, one < '.' >, object_name> {};

    // Matches a list of terms used for invocation a method.
    struct method_list_of_args : opt < list < method_term, comma > >{};

    struct method_invocation_head :
            seq < object_name, one < '.' >, method_name > {};

    // Call of a method, this includes both object method called for,
    // method name and the list of arguments.
    struct method_invocation :
            seq < method_invocation_head,
                    sur_with_brackets < disable < method_list_of_args > > > {};

    // Used just to extract property name.
    struct assignment_prop_name : object_name {};

    // Matches assignment of property like "this.fst = fst".
    // TODO: Validate the same identifier is used on both sides of assignment.
    struct assignment :
            seq <property_invocation, assign, assignment_prop_name > {};

    struct method_term : sor < method_invocation, property_invocation,
            variable_term > {};

    // Required returned value from method. Matches the pattern "return ...".
    struct return_stat : seq < pad < return_keyword, space, space >,
            method_term > {};

    /* Method definitions */

    // Matches single method argument, like "Object x".
    struct method_arg : seq < class_name, space, object_name > {};

    // Matches "Object x, ..." or just nothing for method with no arguments.
    struct method_arguments : opt < list < method_arg, comma > > {};

    // Matches constructor definition, in this case it should match class name.
    struct constructor_name : class_name {};

    // Matches constructor name and its arguments list.
    struct constructor_head : seq < constructor_name,
            sur_with_brackets < method_arguments > > {};

    // Matches the whole content of constructor body.
    // TODO: Throw local error if failed to match
    struct constructor_body : seq < super_invocation, semicolon,
            star_must < assignment, semicolon > > {};

    // Matches the whole constructor definition.
    struct constructor_def : seq < constructor_head,
            sur_with_braces < constructor_body > > {};

    // Matches common method declaration, with return type and name.
    struct method_ret_and_name : seq < class_name, space, method_name > {};

    // Matches the method head and its args list.
    struct method_head : must < method_ret_and_name,
            sur_with_brackets < method_arguments > > {};

    // Matches the content of {"..."}.
    struct method_body : seq < return_stat, semicolon > {};

    // Matches single method definition.
    struct method_def : seq < method_head, sur_with_braces < method_body > > {};

    /* Classes */

    // Matches "class A extends B"
    struct class_header : seq < class_keyword, lexeme < declared_class_name >,
            extends_keyword, lexeme < inherited_class_name > > {};

    // Matches the content of {"..."}.
    struct class_body : seq < list < class_terms, semicolon >, semicolon > {};

    // Matches the whole class body.
    struct class_def : seq< class_header, sur_with_braces< class_body > > {};

    // List of classes.
    struct classes : list < class_def, opt < space > > {};

    // The top-level grammar allows one class definition and then expects eof.
    struct grammar : must < classes, eof > {};
}

#endif //FJ_LEXEMES_H
