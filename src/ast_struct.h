#ifndef FJ_AST_STRUCT_H
#define FJ_AST_STRUCT_H

#include <string>
#include <iostream>

#include <pegtl.hh>

namespace fj {
    // Parsing rule that matches a literal "class".
    struct class_keyword
            : pegtl::string< 'c', 'l', 'a', 's', 's' > {};

    // Parsing rule that matches a literal "extends".
    struct extends_keyword
            : pegtl::string< 'e', 'x', 't', 'e', 'n', 'd', 's' > {};

    // Parsing rule that matches a literal "super".
    struct super_keyword
            : pegtl::string< 's', 'u', 'p', 'e', 'r' > {};

    // Parsing rule that matches a literal "return".
    struct return_keyword
            : pegtl::string< 'r', 'e', 't', 'u', 'r', 'n' > {};

    // Parsing rule that matches a literal "new".
    struct new_keyword
            : pegtl::string< 'n', 'e', 'w' > {};

    // Parsing rule that matches a literal "this".
    struct this_keyword
            : pegtl::string< 't', 'h', 'i', 's' > {};

    // Parsing rule that matches a non-empty sequence of
    // alphabetic ascii-characters with greedy-matching.
    struct class_name
            : pegtl::plus< pegtl::alpha > {};

    // Parsing rule that matches a non-empty sequence of
    // alphabetic ascii-characters with greedy-matching.
    struct method_name
            : pegtl::plus< pegtl::alpha > {};

    // Parsing rule that matches a non-empty sequence of
    // alphabetic ascii-characters with greedy-matching.
    // Could be used for both property and argument names.
    struct object_name
            : pegtl::plus< pegtl::alpha > {};

    struct class_def
            : pegtl::seq< class_keyword, class_name,
                    extends_keyword, class_name,
                    pegtl::string < '{' >,
                    pegtl::string < '}' > > {};
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

int main1( int argc, char * argv[] )
{
    if ( argc > 1 ) {
        // Start a parsing run of argv[1] with the string
        // variable 'name' as additional argument to the
        // action; then print what the action put there.

        std::string name;
        pegtl::parse< hello::grammar, hello::action >( 1, argv, name );
        std::cout << "Good bye, " << name << "!" << std::endl;
    }
}

#endif //FJ_AST_STRUCT_H
