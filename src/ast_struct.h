#ifndef FJ_AST_STRUCT_H
#define FJ_AST_STRUCT_H

#include <string>
#include <iostream>

#include <pegtl.hh>

typedef std::map<std::string, std::string> Properties;

class ClassDeclaration {
public:
    ClassDeclaration(std::string name) {
        this->name = name;
    };
    void setParentName(std::string name) { this->parentName = name; };
    void addProperty(std::string className, std::string propertyName) {
        // TODO: Fail if not unique.
        properties[propertyName] = className;
    };

    std::string getName() const { return name; };
    std::string getParentName() const { return parentName; };
    const Properties *getProperties() const { return &properties; };

private:
    std::string name, parentName;
    Properties properties;
};

class ParsedContext {
public:
    ParsedContext() {};
    void addClass(ClassDeclaration newClass) { classes.push_back(newClass); };
    ClassDeclaration *currentClass() {
        assert((bool)classes.size());
        return &classes.back();
    }
    std::vector< ClassDeclaration > getClasses() const { return classes; }
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
    struct class_name
            : plus< alpha > {};

    // Parsing rule that matches a non-empty sequence of
    // alphabetic ascii-characters with greedy-matching.
    struct method_name
            : plus< alpha > {};

    // Parsing rule that matches a non-empty sequence of
    // alphabetic ascii-characters with greedy-matching.
    // Could be used for both property and argument names.
    struct object_name
            : plus< alnum > {};

    // Parsing rule that matches a non-empty sequence of
    // alphabetic ascii-characters with greedy-matching.
    struct declared_class_name
            : class_name {};

    // Matches the class name after the extends keyword.
    struct inherited_class_name
            : class_name {};

    struct property_def
            : seq< class_name, space, object_name > {};

    struct class_terms
            : sor < property_def > {};

    struct semicolon
            : seq < opt < space >, one < ';' >, opt < space > > {};

    // Ignores leading and trailing spaces.
    template < typename Rule >
    struct lexeme
            : seq < space, Rule, space> {};

    // Matches the pattern like "{Rule}"
    template < typename Rule >
    struct surrounded_with_braces
            : seq < one < '{' >, Rule, one < '}' >> {};

    struct class_body
            : list < class_terms, semicolon > {};

    struct class_def
            : seq< class_keyword, lexeme < declared_class_name >,
                   extends_keyword, lexeme < inherited_class_name >,
                   surrounded_with_braces < class_body > > {};

    // The top-level grammar allows one class definition and then expects eof.
    struct grammar
            : list< class_def, opt < space > > {};

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

    // Process the property name in class declaration.
    template<> struct action< property_def > {
        static void apply( const pegtl::input & in, ParsedContext & context ) {
//            context.currentClass()->addProperty();
            std::cout << "Prop" << in.string() << std::endl;
        }
    };
}

#endif //FJ_AST_STRUCT_H
