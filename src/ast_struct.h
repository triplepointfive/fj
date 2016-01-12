#ifndef FJ_AST_STRUCT_H
#define FJ_AST_STRUCT_H

#include <string>
#include <iostream>

#include <pegtl.hh>
#include <pegtl/contrib/changes.hh>

typedef std::map<std::string, std::string> Properties;
typedef std::map<std::string, std::string> Arguments;
typedef std::map<std::string, std::string> Methods;

class ParsedContext;

class MethodTerm {
public:
    MethodTerm(const std::string &name) : name(name) { }

    std::string getName() const {
        return name;
    }

    MethodTerm(const MethodTerm &) = delete;
    void operator= (const MethodTerm &) = delete;

    // TODO: This is stub for test to check the actual state. Review one day.
    virtual std::string type() const = 0;

    virtual ~MethodTerm() {}

protected:
    std::string name;
};

class PropertyTerm : public MethodTerm {
public:
    PropertyTerm(std::string propertyName) : MethodTerm(propertyName) {}
    std::string type() const override { return "property"; }
};

class VariableTerm : public MethodTerm {
public:
    VariableTerm(std::string variableName) : MethodTerm(variableName) {}
    std::string type() const override { return "variable"; }
};

class ArgumentTerm : public MethodTerm {
public:
    ArgumentTerm(std::string name) : MethodTerm(name) {};
    void addArg(MethodTerm *term) { terms.push_back(term); }

    const std::vector<MethodTerm *> *getArgs() const { return &terms; }

    virtual ~ArgumentTerm() {
        for (auto term : terms) {
            delete term;
        }
    }

private:
    std::vector<MethodTerm *> terms;
};

class MethodInvocation : public ArgumentTerm {
public:
    MethodInvocation(const std::string &objectName,
                     const std::string &methodName)
        : ArgumentTerm(methodName)
        , objectName(objectName) { }
    std::string type() const override { return "method"; }

    std::string getObjectName() const { return objectName; }
private:
    std::string objectName;
};

class BaseMethod {
public:
    void setName(const std::string &name) { this->name = name; }

    void addArg(std::string name, std::string class_name) {
        // TODO: Validate uniqueness
        args[name] = class_name;
    }

    Arguments getArgs() const { return args; }
    std::string getName() const { return name; }

protected:
    std::string name;
    Arguments args;
};

class ConstructorBody : public BaseMethod {
public:
    ConstructorBody() {};

    void addProperty(const std::string &propertyName) {
        properties.push_back(propertyName);
    }

    std::vector<std::string> getProperties() const { return properties; }

    void success(ParsedContext & context) {
        std::cout << "success GOT CALLED: " << std::endl;
    }

private:
    std::vector<std::string> properties;
};

class MethodDeclaration : public BaseMethod {
public:
    MethodDeclaration(const std::string &name,
                      const std::string &return_class_name)
        : return_class_name(return_class_name) {
        this->name = name;
    }

    void setBodyTerm(MethodTerm *methodTerm) {
        this->treeHead = methodTerm;
    }

    MethodTerm *getBodyTerm() const { return treeHead; }

    void setReturnClassName(const std::string &return_class_name) {
        this->return_class_name = return_class_name;
    }

    std::string getReturnClassName() const { return return_class_name; }

    ~MethodDeclaration() {
        if (nullptr != treeHead) {
            delete treeHead;
        }
    }

private:
    std::string return_class_name;
    MethodTerm *treeHead{ nullptr };
};

class ClassDeclaration {
public:
    ClassDeclaration(const std::string &name) : name(name) {};
    ClassDeclaration() {};
    ~ClassDeclaration() {
        if (nullptr != constructorBody) {
            delete constructorBody;
        }
    }

    void success(ParsedContext & context);

    void setName(const std::string &name) { this->name = name; };
    void setParentName(const std::string &name) { this->parentName = name; };
    void addProperty(const std::string &className,
                     const std::string &propertyName) {
        // TODO: Fail if not unique.
        properties[propertyName] = className;
    };
    void addMethod(MethodDeclaration *methodDeclaration) {
        methods.push_back(methodDeclaration);
    }
    void addMethod(const std::string &method_name,
                   const std::string &return_class_name) {
        methods.push_back(
            new MethodDeclaration(method_name, return_class_name)
        );
    };

    void setConstructorBody(ConstructorBody *constructorBody) {
        // TODO: Fail if already presence - means constructor is defined twice.
        this->constructorBody = constructorBody;
    }

    ConstructorBody *getConstructorBody() const {
        return constructorBody;
    }

    MethodDeclaration *currentMethod() {
        assert((bool)methods.size());
        return methods.back();
    }

    std::vector<MethodDeclaration*> getMethods() const { return methods; }
    std::string getName() const { return name; };
    std::string getParentName() const { return parentName; };
    const Properties *getProperties() const { return &properties; };

private:
    std::string name, parentName;
    Properties properties;
    std::vector<MethodDeclaration*> methods;
    ConstructorBody *constructorBody{ nullptr };
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

void ClassDeclaration::success(ParsedContext & context) {
    std::cout << "ClassDeclaration: success" << std::endl;
    context.addClass(*this);
}

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

    // The top-level grammar allows one class definition and then expects eof.
    struct grammar : until< eof, list < class_def, opt < space > > > {};

    void splitOn(std::string origin, std::string delimiter,
                 std::string &fst, std::string &snd) {
        unsigned long space = origin.find(delimiter);
        fst = origin.substr(0, space);
        snd = origin.substr(space + 1, origin.size());
    }

    void splitOnSpace(std::string origin, std::string &fst, std::string &snd) {
        // TODO: Assumes delimiter is a space, should be more abstract.
        splitOn(origin, " ", fst, snd);
    }

    void splitOnDot(std::string origin, std::string &fst, std::string &snd) {
        splitOn(origin, ".", fst, snd);
    }

    template< typename Rule >
    struct build_method_body : pegtl::nothing< Rule > {};

    template< typename Rule >
    struct build_method_invocation : pegtl::nothing< Rule > {};

    template<> struct build_method_invocation< method_term > {
        static void apply(const pegtl::input & in,
                          MethodInvocation **methodInvocation) {
            std::cout << "build_method_invocation: method_term: " << in.string() << std::endl;
        }
    };

    template<> struct build_method_invocation< method_invocation_head > {
        static void apply(const pegtl::input & in,
                          MethodInvocation **methodInvocation) {
            std::cout << "build_method_invocation: method_invocation_head: " << in.string() << std::endl;
            std::string objectName, methodName;
            splitOnDot(in.string(), objectName, methodName);
            *methodInvocation = new MethodInvocation(objectName, methodName);
        }
    };

    template<> struct build_method_invocation< method_list_of_args > {
        static void apply(const pegtl::input &in,
                          MethodInvocation **methodInvocation) {
            std::cout << "build_method_invocation: method_list_of_args: " << in.string() << std::endl;
        }
    };

    template<> struct build_method_body< method_invocation > {
        static void apply( const pegtl::input & in, MethodTerm **methodTerm) {
            std::cout << "build_method_body: method_invocation: " << in.string() << std::endl;
            MethodInvocation *methodInvocation = nullptr;
            // TODO: Validate returned status.
            pegtl::parse< fj::method_invocation, fj::build_method_invocation >(
                in.string(),
                "method_body rule",
                &methodInvocation
            );
            *methodTerm = methodInvocation;
        }
    };

    template<> struct build_method_body< property_invocation > {
        static void apply( const pegtl::input & in, MethodTerm **methodTerm) {
            std::cout << "build_method_body: property_term: " << in.string() << std::endl;
            std::string thisStr, propName;
            splitOnDot(in.string(), thisStr, propName);
            *methodTerm = new PropertyTerm(propName);
        }
    };

    template<> struct build_method_body< variable_term > {
        static void apply( const pegtl::input & in, MethodTerm **methodTerm) {
            std::cout << "build_method_body: variable_term: " << in.string() << std::endl;
            *methodTerm = new VariableTerm(in.string());
        }
    };

    template< typename Rule >
    struct build_constructor : pegtl::nothing< Rule > {};

    template<> struct build_constructor< constructor_name > {
        static void apply( const pegtl::input & in, ConstructorBody & constr ) {
            constr.setName(in.string());
        }
    };

    template<> struct build_constructor< assignment_prop_name > {
        static void apply( const pegtl::input & in, ConstructorBody & constr ) {
            constr.addProperty(in.string());
        }
    };

    template<> struct build_constructor< method_arg > {
        static void apply( const pegtl::input & in, ConstructorBody & constr ) {
            std::cout << "method_arg: " << in.string() << std::endl;
            std::string className, argName;
            splitOnSpace(in.string(), className, argName);
            constr.addArg(argName, className);
        }
    };

    template< typename Rule >
    struct build_method : pegtl::nothing< Rule > {};

    template<> struct build_method< method_ret_and_name > {
        static void apply(const pegtl::input & in, MethodDeclaration & method) {
            std::cout << "method_ret_and_name: " << in.string() << std::endl;
            std::string className, methodName;
            splitOnSpace(in.string(), className, methodName);
            method.setName(methodName);
            method.setReturnClassName(className);
        }
    };

    template<> struct build_method< method_arg > {
        static void apply(const pegtl::input & in, MethodDeclaration & method) {
            std::cout << "method_arg: " << in.string() << std::endl;
            std::string className, argName;
            splitOnSpace(in.string(), className, argName);
            method.addArg(argName, className);
        }
    };

    // Assigns top node of body tree to the current method declaration.
    template<> struct build_method< method_body > {
        static void apply(const pegtl::input & in, MethodDeclaration & method) {
            MethodTerm *methodTerm = nullptr;
            // TODO: Validate returned status.
            pegtl::parse< fj::method_body, fj::build_method_body >(
                in.string(),
                "method_body rule",
                &methodTerm
            );
            method.setBodyTerm(methodTerm);
        }
    };

    template< typename Rule >
    struct build_class : pegtl::nothing< Rule > {};

    template<> struct build_class< declared_class_name > {
        static void apply(const pegtl::input & in, ClassDeclaration & classDeclaration) {
            std::cout << "declared_class_name: " << in.string() << std::endl;
            classDeclaration.setName(in.string());
        }
    };

    template<> struct build_class< inherited_class_name > {
        static void apply(const input & in, ClassDeclaration & classDeclaration) {
            std::cout << "declared_class_name: " << in.string() << std::endl;
            classDeclaration.setParentName(in.string());
        }
    };

    // Top level action for parser.
    template< typename Rule >
    struct build_grammar : pegtl::nothing< Rule > {};

    // Init new class with given name.
    template<> struct build_grammar< declared_class_name > {
        static void apply( const pegtl::input & in, ParsedContext & context ) {
            std::cout << "declared_class_name: " << in.string() << std::endl;
            context.addClass(ClassDeclaration(in.string()));
        }
    };

    // Assign the parent class name with current class.
    template<> struct build_grammar< inherited_class_name > {
        static void apply( const pegtl::input & in, ParsedContext & context ) {
            std::cout << "inherited_class_name: " << in.string() << std::endl;
            context.currentClass()->setParentName(in.string());
        }
    };

    // Process the property name in class declaration.
    template<> struct build_grammar< property_def > {
        static void apply( const pegtl::input & in, ParsedContext & context ) {
            std::cout << "property_def: " << in.string() << std::endl;
            std::string className, propertyName;
            splitOnSpace(in.string(), className, propertyName);
            context.currentClass()->addProperty(className, propertyName);
        }
    };

    // Creates new constructor for current class.
    template<> struct build_grammar< constructor_def > {
        static void apply( const pegtl::input & in, ParsedContext & context ) {
            ConstructorBody *constructorBody = new ConstructorBody;
            // TODO: Validate returned status.
            pegtl::parse< fj::constructor_def, fj::build_constructor >(
                in.string(),
                "constructor_def rule",
                *constructorBody
            );

            context.currentClass()->setConstructorBody(constructorBody);
        }
    };

    // Creates new method for current class.
    template<> struct build_grammar< method_def > {
        static void apply( const pegtl::input & in, ParsedContext & context ) {
            // TODO: Check if possible to do without stubs.
            MethodDeclaration *methodDeclaration =
                new MethodDeclaration("stubName", "stubRetObject");
            // TODO: Validate returned status.
            pegtl::parse< fj::method_def, fj::build_method >(
                in.string(),
                "method_def rule",
                *methodDeclaration
            );

            context.currentClass()->addMethod(methodDeclaration);
        }
    };

    template< typename Rule > struct control : normal< Rule > {};
    template<> struct control< class_def > : change_state_and_action< class_def, ClassDeclaration, build_class > {};
}

#endif //FJ_AST_STRUCT_H
