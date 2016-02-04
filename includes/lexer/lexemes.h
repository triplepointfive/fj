#ifndef FJ_LEXEMES_H
#define FJ_LEXEMES_H

#include <pegtl.hh>

#define pegtl_char_t(x) pad < one < x >, space, space >

namespace fj {
    using namespace pegtl;

    /* Keywords */

    struct class_keyword   : pegtl_string_t("class")   {};
    struct extends_keyword : pegtl_string_t("extends") {};
    struct super_keyword   : pegtl_string_t("super")   {};
    struct return_keyword  : pad < pegtl_string_t("return"), space > {};
    struct new_keyword     : pegtl_string_t("new")     {};
    struct this_keyword    : pegtl_string_t("this")    {};

    /* Special characters with optional paddies */

    struct semicolon     : pegtl_char_t(';') {};
    struct open_brace    : pegtl_char_t('{') {};
    struct close_brace   : pegtl_char_t('}') {};
    struct open_bracket  : pegtl_char_t('(') {};
    struct close_bracket : pegtl_char_t(')') {};
    struct comma         : pegtl_char_t(',') {};
    struct assign        : pegtl_char_t('=') {};
    struct dot           : pegtl_char_t('.') {};

    /* Identifiers */

    // TODO: Exclude keywords
    struct class_name  : plus< identifier > {};
    struct method_name : plus< identifier > {};
    struct object_name : plus< identifier > {};

    /* Meta rules */

    // Matches the pattern like "{Rule}"
    template < typename Rule >
    struct sur_with_braces : must < open_brace, Rule, close_brace > {};

    // Matches the pattern like "(Rule)"
    template < typename Rule >
    struct sur_with_brackets : seq < open_bracket, Rule, close_bracket > {};

    /* Method arguments */

    // Matches single method argument, like "Object x".
    struct method_arg : seq < class_name, space, object_name > {};

    // Matches "Object x, ..." or just nothing for method with no arguments.
    struct method_arguments : opt < list < method_arg, comma > > {};

    /* Method terms */

    struct method_term; // Forwarding declaration for cycle usage.

    // Just a variable name, matching general restrictions.
    struct variable_term : object_name {};

    // Left part of assignment, matches "this.fst".
    struct property_invocation : seq < this_keyword, dot, object_name> {};

    // Matches a list of terms used for invocation a method.
    struct method_list_of_args : opt < list < method_term, comma > >{};

    // TODO: Make more strict
    struct method_invocation_head : seq < object_name, dot, method_name > {};

    // Call of a method, this includes both object method called for,
    // method name and the list of arguments.
    struct method_invocation :
            seq < method_invocation_head,
                    sur_with_brackets < method_list_of_args > > {};

    struct instantiation_class : class_name {};
    // TODO: Make more strict
    struct instantiation_head : seq < new_keyword, space, instantiation_class > {};

    // Creation of new object, includes "new" keyword and a list of arguments.
    struct instantiation :
        seq < instantiation_head, sur_with_brackets < method_list_of_args > > {};

    struct type_casting_class : class_name {};
    // Cast type of object to one its parent classes.
    struct type_casting : seq < sur_with_brackets < type_casting_class >,
        method_term > {};

    // Used just to extract property name.
    struct assignment_prop_name : object_name {};

    // Matches assignment of property like "this.fst = fst".
    // TODO: Validate the same identifier is used on both sides of assignment.
    struct assignment :
            seq <property_invocation, assign, assignment_prop_name > {};

    struct attribute_access_term : seq < this_keyword, dot, object_name > {};
    struct method_term : sor < type_casting, instantiation, method_invocation,
        attribute_access_term, variable_term > {};

    // Just for better error message handling.
    struct returned_statement : method_term {};
    // Required returned value from method. Matches the pattern "return ...".
    struct return_stat : if_must < return_keyword, returned_statement > {};

    /* Constructors */

    struct constructor_name : class_name {};

    // Matches constructor name and its arguments list.
    struct constructor_head : seq < constructor_name,
            sur_with_brackets < method_arguments > > {};

    struct super_arg : object_name {};
    struct super_args : opt < list < super_arg, comma > > {};

    // Matches invocation of super.
    struct super_invocation :
        seq < super_keyword, sur_with_brackets < super_args > > {};

    struct properties_list : star_must < assignment, semicolon > {};

    // Matches the whole content of constructor body.
    struct constructor_body : must < super_invocation, semicolon,
        properties_list > {};

    // Matches the whole constructor definition.
    struct constructor_def : seq < constructor_head,
            sur_with_braces < constructor_body > > {};

    /* Methods */

    struct met_class : class_name {};
    struct met_name : method_name {};

    // Matches common method declaration, with return type and name.
    struct method_ret_and_name : seq < met_class, space, met_name > {};

    // Matches the method head and its args list.
    struct method_head : seq < method_ret_and_name,
            sur_with_brackets < method_arguments > > {};

    // Matches the content of {"..."}.
    struct method_body : must < return_stat, semicolon > {};

    // Matches single method definition.
    struct method_def : if_must < method_head, sur_with_braces < method_body > > {};

    /* Classes */

    // Matches the name of new class.
    struct declared_class_name : class_name {};

    // Matches the class name after the extends keyword.
    struct inherited_class_name : class_name {};

    struct declared_class_name_padded : pad < declared_class_name, space > {};
    struct inherited_class_name_padded : pad < inherited_class_name, space > {};

    // Matches "class A extends B"
    struct class_header : if_must < class_keyword, declared_class_name_padded,
        extends_keyword, inherited_class_name_padded > {};

    // Matches new property inside class body.
    struct property_def :
        if_must < seq < class_name, space, object_name >, semicolon > {};

    // A list of base units inside class body: properties, methods etc.
    struct class_term : sor < constructor_def, method_def, property_def > {};

    // Matches the content of {"..."}.
    struct class_body : list < class_term, star< space > > {};

    // Matches the whole class body.
    struct class_def : seq< class_header, sur_with_braces< class_body > > {};

    // List of classes.
    struct classes : list < class_def, success, space > {};

    // The top-level grammar allows one class definition and then expects eof.
    struct grammar : must < classes, eof > {};
}

#endif //FJ_LEXEMES_H
