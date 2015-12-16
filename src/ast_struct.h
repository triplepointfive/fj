#ifndef FJ_AST_STRUCT_H
#define FJ_AST_STRUCT_H

#include <string>
#include <iostream>

#include <pegtl.hh>

namespace fj {
    using namespace pegtl;
    // Comments are introduced by a '#' and proceed to the end-of-line/file.
    struct comment
            : if_must< two< '/' >, until< eolf > > {};

    // The calculator ignores all spaces and comments; space is a pegtl rule
    // that matches the usual ascii characters ' ', '\t', '\n' etc. In other
    // words, everything that is space or a comment is ignored.
    struct ignored
            : sor< space, comment > {};

    // Parsing rule that matches a literal "class".
    struct class_keyword
            : string< 'c', 'l', 'a', 's', 's' > {};

    // Parsing rule that matches a literal "extends".
    struct extends_keyword
            : string< 'e', 'x', 't', 'e', 'n', 'd', 's' > {};

    // Parsing rule that matches a literal "super".
    struct super_keyword
            : string< 's', 'u', 'p', 'e', 'r' > {};

    // Parsing rule that matches a literal "return".
    struct return_keyword
            : string< 'r', 'e', 't', 'u', 'r', 'n' > {};

    // Parsing rule that matches a literal "new".
    struct new_keyword
            : string< 'n', 'e', 'w' > {};

    // Parsing rule that matches a literal "this".
    struct this_keyword
            : string< 't', 'h', 'i', 's' > {};

    // Parsing rule that matches a non-empty sequence of
    // alphabetic ascii-characters with greedy-matching.
    struct class_name
            : seq < space, plus< alpha >, space> {};

    // Parsing rule that matches a non-empty sequence of
    // alphabetic ascii-characters with greedy-matching.
    struct method_name
            : plus< alpha > {};

    // Parsing rule that matches a non-empty sequence of
    // alphabetic ascii-characters with greedy-matching.
    // Could be used for both property and argument names.
    struct object_name
            : plus< alpha > {};

    struct class_def
            : seq< class_keyword, class_name > {};
//                    extends_keyword, class_name,
//                    one < '{' >,
//                    one < '}' > > {};

//    struct class_def
//            : seq< class_keyword, class_name,
//                    extends_keyword, class_name,
//                    one < '{' >,
//                    one < '}' > > {};

    // The top-level grammar allows one class defenition and then expects eof.
    struct grammar
            : must< class_def, eof > {};

    // Class template for user-defined actions that does
    // nothing by default.

    template< typename Rule >
    struct action
            : pegtl::nothing< Rule > {};

    // Specialisation of the user-defined action to do
    // something when the 'name' rule succeeds; is called
    // with the portion of the input that matched the rule.

    template<> struct action< class_name >
    {
        static void apply( const pegtl::input & in, std::string & name )
        {
            name = in.string();
        }
    };
}

namespace hello
{
    // Parsing rule that matches a literal "Hello, ".

    struct prefix
            : pegtl::string< 'H', 'e', 'l', 'l', 'o', ',', ' ' > {};

    // Parsing rule that matches a non-empty sequence of
    // alphabetic ascii-characters with greedy-matching.

    struct name
            : pegtl::plus< pegtl::alpha > {};

    // Parsing rule that matches a sequence of the 'prefix'
    // rule, the 'name' rule, a literal "!", and 'eof'
    // (end-of-file/input), and that throws an exception
    // on failure.

    struct grammar
            : pegtl::must< prefix, name, pegtl::one< '!' >, pegtl::eof > {};

    // Class template for user-defined actions that does
    // nothing by default.

    template< typename Rule >
    struct action
            : pegtl::nothing< Rule > {};

    // Specialisation of the user-defined action to do
    // something when the 'name' rule succeeds; is called
    // with the portion of the input that matched the rule.

    template<> struct action< name >
    {
        static void apply( const pegtl::input & in, std::string & name )
        {
            name = in.string();
        }
    };

} // hello

#endif //FJ_AST_STRUCT_H
