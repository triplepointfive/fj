#ifndef FJ_AST_STRUCT_H
#define FJ_AST_STRUCT_H

#include <string>
#include <iostream>

#include <pegtl.hh>

typedef std::map<std::string, std::string> Properties;
typedef std::map<std::string, std::string> Arguments;
typedef std::map<std::string, std::string> Methods;

class MethodDeclaration {
public:
    MethodDeclaration(std::string name, std::string return_class_name) {
        this->name = name;
        this->return_class_name = return_class_name;
    }

    void addArg(std::string name, std::string class_name) {
        // TODO: Validate uniqueness
        args[name] = class_name;
    }

    Arguments getArgs() const { return args; }
    std::string getName() const { return name; }
    std::string getReturnClassName() const { return return_class_name; }

private:
    std::string name, return_class_name;
    Arguments args;
};

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
    void addMethod(std::string method_name, std::string return_class_name) {
        methods.push_back(MethodDeclaration(method_name, return_class_name));
    };

    MethodDeclaration *currentMethod() {
        assert((bool)methods.size());
        return &methods.back();
    }

    std::vector<MethodDeclaration> getMethods() const { return methods; }
    std::string getName() const { return name; };
    std::string getParentName() const { return parentName; };
    const Properties *getProperties() const { return &properties; };

private:
    std::string name, parentName;
    Properties properties;
    std::vector<MethodDeclaration> methods;
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

    /* Keywords */

    // Parsing rule that matches a literal "class".
    struct class_keyword : string< 'c', 'l', 'a', 's', 's' > {};

    // Parsing rule that matches a literal "extends".
    struct extends_keyword : string< 'e', 'x', 't', 'e', 'n', 'd', 's' > {};

    // Parsing rule that matches a literal "super".
    struct super_keyword : string< 's', 'u', 'p', 'e', 'r' > {};

    // Parsing rule that matches a literal "return".
    struct return_keyword : string< 'r', 'e', 't', 'u', 'r', 'n' > {};

    // Parsing rule that matches a literal "new".
    struct new_keyword : string< 'n', 'e', 'w' > {};

    // Parsing rule that matches a literal "this".
    struct this_keyword : string< 't', 'h', 'i', 's' > {};

    /* Special characters with optional paddes */

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

    // Matches invocation of super.
    struct super_invocation :
            seq < super_keyword, sur_with_brackets < success > > {};

    // Matches assignment of property like "this.fst = fst".
    struct assignment : seq < this_keyword, one < '.' >,
            object_name, assign, object_name > {};

    // Matches the whole content of constructor body.
    struct constructor_body : sor < super_invocation, assignment > {};

    // Matches single method argument, like "Object x".
    struct method_arg : seq < class_name, space, object_name > {};

    // Matches "Object x, ..." or just nothing for method with no arguments.
    struct method_arguments : opt < list < method_arg, comma > > {};

    // Matches constructor definition, in this case it should match class name.
    struct constructor_head : class_name {};

    // Matches common method declaration, with return type and name.
    struct method_ret_and_name : seq < class_name, space, method_name > {};

    // Matches the method head and its args list.
    struct method_head : must < sor < method_ret_and_name, constructor_head >,
            sur_with_brackets < method_arguments > > {};

    // Matches the content of {"..."}.
    struct method_body : seq < list < class_terms, semicolon >, semicolon > {};

    // Matches single method definition.
    struct method_def : seq < method_head, sur_with_braces < method_body > > {};

    // Matches "class A extends B"
    struct class_header : seq < class_keyword, lexeme < declared_class_name >,
           extends_keyword, lexeme < inherited_class_name > > {};

    // Matches the content of {"..."}.
    struct class_body : seq < list < class_terms, semicolon >, semicolon > {};

    // Matches the whole class body.
    struct class_def : seq< class_header, sur_with_braces< class_body > > {};

    // The top-level grammar allows one class definition and then expects eof.
    struct file : until< eof, list < class_def, opt < space > > > {};


    void splitOnSpace(std::string origin, std::string &fst, std::string &snd) {
        // TODO: Assumes delimiter is a space, should be more abstract.
        unsigned long space = origin.find(" ");
        fst = origin.substr(0, space);
        snd = origin.substr(space + 1, origin.size());
    }

    // Top level action for parser.
    template< typename Rule >
    struct action
            : pegtl::nothing< Rule > {};

    // Init new class with given name.
    template<> struct action< declared_class_name > {
        static void apply( const pegtl::input & in, ParsedContext & context ) {
            std::cout << "declared_class_name: " << in.string() << std::endl;
            context.addClass(ClassDeclaration(in.string()));
        }
    };

    // Assign the parent class name with current class.
    template<> struct action< inherited_class_name > {
        static void apply( const pegtl::input & in, ParsedContext & context ) {
            std::cout << "inherited_class_name: " << in.string() << std::endl;
            context.currentClass()->setParentName(in.string());
        }
    };

    // Process the property name in class declaration.
    template<> struct action< property_def > {
        static void apply( const pegtl::input & in, ParsedContext & context ) {
            std::cout << "property_def: " << in.string() << std::endl;
            std::string className, propertyName;
            splitOnSpace(in.string(), className, propertyName);
            context.currentClass()->addProperty(className, propertyName);
        }
    };

    // Process method argument.
    template<> struct action< method_arg > {
        static void apply( const pegtl::input & in, ParsedContext & context ) {
            std::cout << "method_arg: " << in.string() << std::endl;
            std::string className, argName;
            splitOnSpace(in.string(), className, argName);
            context.currentClass()->currentMethod()->addArg(argName, className);
        }
    };

    // Creates new constructor for current class.
    template<> struct action< constructor_head > {
        static void apply( const pegtl::input & in, ParsedContext & context ) {
            std::cout << "constructor_head: " << in.string() << std::endl;
            std::string constructorName = in.string();
            context.currentClass()->addMethod(constructorName, constructorName);
        }
    };

    // Creates new constructor for current class.
    template<> struct action< method_ret_and_name > {
        static void apply( const pegtl::input & in, ParsedContext & context ) {
            std::cout << "method_ret_and_name: " << in.string() << std::endl;
            std::string className, methodName;
            splitOnSpace(in.string(), className, methodName);
            context.currentClass()->addMethod(methodName, className);
        }
    };
}

#endif //FJ_AST_STRUCT_H
