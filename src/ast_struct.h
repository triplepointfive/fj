#ifndef FJ_AST_STRUCT_H
#define FJ_AST_STRUCT_H

#include <string>
#include <iostream>

#include <pegtl.hh>

class ClassDeclaration {
public:
    ClassDeclaration(std::string name) {
        this->name = name;
    };
    void setParentName(std::string name) { this->parent_name = name; };

    std::string getName() const { return name; };
    std::string getParentName() const { return parent_name; };

private:
    std::string name, parent_name;
};

class ParsedContext {
public:
    ParsedContext() {};
    void addClass(ClassDeclaration newClass) { classes.push_back(newClass); };
    ClassDeclaration *currentClass() {
        assert((bool)classes.size());
        return &classes.back();
    }
private:
    std::vector< ClassDeclaration > classes;
};

namespace fj {
    using namespace pegtl;

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
    struct declared_class_name
            : plus< alpha > {};

    // Matches the class name after the extends keyword.
    struct inherited_class_name
            : plus< alpha > {};

    // Parsing rule that matches a non-empty sequence of
    // alphabetic ascii-characters with greedy-matching.
    struct method_name
            : plus< alpha > {};

    // Parsing rule that matches a non-empty sequence of
    // alphabetic ascii-characters with greedy-matching.
    // Could be used for both property and argument names.
    struct object_name
            : plus< alpha > {};

    // Ignores leading and trailing spaces.
    template < typename Rule >
    struct lexeme
            : seq < space, Rule, space> {};

    // Matches the pattern like "{Rule}"
    template < typename Rule >
    struct surrounded_with_braces
            : seq < one < '{' >, Rule, one < '}' >> {};

    struct class_body
            : surrounded_with_braces < space > {};

    struct class_def
            : seq< class_keyword, lexeme < declared_class_name >,
                   extends_keyword, lexeme < inherited_class_name > > {};
//                   class_body > {};

    // The top-level grammar allows one class definition and then expects eof.
    struct grammar
            : must< class_def, eof > {};

    // Top level action for parser.
    template< typename Rule >
    struct action
            : pegtl::nothing< Rule > {};

    // Init new class with given name.
    template<> struct action< declared_class_name > {
        static void apply( const pegtl::input & in, ParsedContext & context ) {
            context.addClass(ClassDeclaration(in.string()));
        }
    };

    // Assign the parent class name with current class.
    template<> struct action< inherited_class_name > {
        static void apply( const pegtl::input & in, ParsedContext & context ) {
            context.currentClass()->setParentName(in.string());
        }
    };
}

#endif //FJ_AST_STRUCT_H
