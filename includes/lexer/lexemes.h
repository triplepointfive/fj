#ifndef FJ_LEXEMES_H
#define FJ_LEXEMES_H

#include <pegtl.hh>

#define pegtl_char_t(x) pad < one < x > >

namespace fj {
    using namespace pegtl;

    /* Parse meta */

    struct comment : disable< two< '/' >, until< eolf > > {};

    struct sep : sor< space, comment > {};
    struct seps : star< sep > {};

    template< typename R >
    struct pad : pegtl::pad< R, sep > {};

    /* Keywords */

    struct str_class   : pegtl_string_t("class")   {};
    struct str_extends : pegtl_string_t("extends") {};
    struct str_super   : pegtl_string_t("super")   {};
    struct str_return  : pegtl_string_t("return")  {};
    struct str_new     : pegtl_string_t("new")     {};
    struct str_this    : pegtl_string_t("this")    {};

    struct str_keyword : sor< str_class, str_extends, str_super,
        str_return, str_new, str_this > {};

    template< typename Key >
    struct key : pegtl::seq< Key, pegtl::not_at< pegtl::identifier_other > > {};

    struct class_keyword   : key< str_class >   {};
    struct extends_keyword : key< str_extends > {};
    struct super_keyword   : key< str_super >   {};
    struct return_keyword  : key< str_return >  {};
    struct new_keyword     : key< str_new >     {};
    struct this_keyword    : key< str_this >    {};

    struct keyword : key< str_keyword > {};

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

    struct class_name  : seq< not_at < keyword >, plus< identifier > > {};
    struct method_name : seq< not_at < keyword >, plus< identifier > > {};
    struct object_name : seq< not_at < keyword >, plus< identifier > > {};

    /* Meta rules */

    // Matches the pattern like "{Rule}"
    template < typename Rule >
    struct sur_with_braces : must < open_brace, Rule, close_brace > {};

    // Matches the pattern like "(Rule)"
    template < typename Rule >
    struct sur_with_brackets : if_must < open_bracket, Rule, close_bracket > {};

    /* Method arguments */

    // Matches single method argument, like "Object x".
    struct method_arg : if_must < class_name, seps, object_name > {};

    // Matches "Object x, ..." or just nothing for method with no arguments.
    struct method_arguments : opt < list < method_arg, comma > > {};

    /* Method terms */

    struct method_term; // Forwarding declaration for cycle usage.

    // General variable name available in method's body.
    struct variable_name : sor < this_keyword, object_name > {};

    // Just a variable name, matching general restrictions.
    struct variable_term : variable_name {};

    // Left part of assignment, matches "this.fst".
    struct property_invocation : seq < this_keyword, dot, object_name> {};

    // Matches a list of terms used for invocation a method.
    struct method_list_of_args : opt < list < method_term, comma > >{};


    struct instantiation_class : class_name {};
    // TODO: Make more strict
    struct instantiation_head : seq < new_keyword, seps, instantiation_class > {};

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

    struct attribute_name : object_name {};
    // TODO: STRICT!
    struct method_term_access;
    // Call of a method, this includes both object method called for,
    // method name and the list of arguments.
    struct access_term : seq< dot, attribute_name, method_term_access > {};
    struct method_invocation : seq< dot, method_name, sur_with_brackets < method_list_of_args >, method_term_access > {};
    struct method_term_access : sor< method_invocation, access_term, success > {};
    struct method_term : seq < sor < type_casting, instantiation, variable_term >, method_term_access > {};

    // Just for better error message handling.
    struct returned_statement : method_term {};
    // Required returned value from method. Matches the pattern "return ...".
    struct return_stat : if_must < return_keyword, seps, returned_statement > {};

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
    struct met_name  : method_name {};

    // Matches common method declaration, with return type and name.
    struct method_ret_and_name : seq < met_class, seps, met_name > {};

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

    struct declared_class_name_padded : pad < declared_class_name > {};
    struct inherited_class_name_padded : pad < inherited_class_name > {};

    // Matches "class A extends B"
    struct class_header : if_must < class_keyword, declared_class_name_padded,
        extends_keyword, inherited_class_name_padded > {};

    // Matches new property inside class body.
    struct property_def :
        if_must < seq < class_name, seps, object_name >, semicolon > {};

    // A list of base units inside class body: properties, methods etc.
    struct class_term : sor < constructor_def, method_def, property_def > {};

    // Matches the content of {"..."}.
    struct class_body : list < class_term, seps > {};

    // Matches the whole class body.
    struct class_def : seq< class_header, sur_with_braces< class_body > > {};

    // List of classes.
    struct classes : list < class_def, success, space > {};

    // The top-level grammar allows one class definition and then expects eof.
    struct grammar : must < classes, eof > {};
}

#endif //FJ_LEXEMES_H
