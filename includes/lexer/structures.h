#ifndef FJ_STRUCTURES_H
#define FJ_STRUCTURES_H

#include <string>
#include <iostream>
#include <map>
#include <vector>
#include <assert.h>
#include <memory>

namespace fj {
    class LexerTermVisitor;

    using Arguments = std::map<std::string, std::string>;

    // TODO: Move to somewhere.
    class non_copyable {
    protected:
        non_copyable() = default;
        ~non_copyable() = default;

        non_copyable(non_copyable const &) = delete;
        void operator=(non_copyable const &x) = delete;
    };

    class MethodTerm {
    public:
        MethodTerm(const std::string &name) : name(name) { }
        MethodTerm() { }
        void setName(std::string name) { this->name = name; }
        std::string getName() const;
        virtual ~MethodTerm() {}
        virtual void accept(LexerTermVisitor* visitor) const = 0;

        // TODO: Rethink of it.
        virtual void setTerm(std::shared_ptr< MethodTerm >) {
            throw "Should never get there";
        }

        // TODO: Debug only
        virtual std::string termType() = 0;
        virtual std::string inspect() = 0;
    protected:
        std::string name;
    };

    class VariableTerm : public MethodTerm {
    public:
        std::string termType() { return "VariableTerm"; };
        std::string inspect() { return name; };
        void accept(LexerTermVisitor* visitor) const override;
    };

    class AccessTerm : public MethodTerm {
    public:
        std::string termType() { return "PropertyTerm"; };
        std::string inspect() {
            if (nullptr != methodTerm) {
                return methodTerm->inspect() + "." + name;
            } else {
                return "{nullptr}." + name;
            }
        };
        void setTerm(std::shared_ptr<MethodTerm> methodTerm) override;
        std::shared_ptr< MethodTerm > getTerm() const {
            return std::move(methodTerm);
        }
        void accept(LexerTermVisitor* visitor) const override;
    private:
        std::shared_ptr< MethodTerm > methodTerm { nullptr };
    };

    class TypeCastingTerm : public MethodTerm {
    public:
        std::string termType() { return "TypeCastingTerm"; };
        std::string inspect() { return "(" + name + ") " + term->inspect(); };
        void setTerm(std::shared_ptr< MethodTerm > term) {
            this->term = term;
        }

        std::shared_ptr< MethodTerm > getTerm() const {
            return std::move(term);
        }
        void accept(LexerTermVisitor* visitor) const override;
    private:
        std::shared_ptr< MethodTerm > term{ nullptr };
    };

    class ArgumentTerm : public MethodTerm {
    public:
        void addArg(std::shared_ptr< MethodTerm > term) { terms.push_back(term); }

        std::vector<std::shared_ptr< MethodTerm > > getArgs() const {
            return terms;
        }
    private:
        std::vector<std::shared_ptr< MethodTerm > > terms;
    };

    class MethodInvocation : public ArgumentTerm {
    public:
        std::string termType() { return "MethodInvocation"; };
        std::string inspect() { return getTerm()->inspect() + "." + name; };

        // TODO: Implement setup.
        void setTerm(std::shared_ptr< MethodTerm > term) {
            this->methodTerm = term;
        }
        std::shared_ptr< MethodTerm > getTerm() const {
            return std::move(methodTerm);
        }
        void accept(LexerTermVisitor* visitor) const override;
    private:
        std::shared_ptr< MethodTerm > methodTerm;
    };

    class Initiation : public ArgumentTerm {
    public:
        std::string termType() { return "Initiation"; };
        std::string inspect() { return "new " + name; };
        void accept(LexerTermVisitor* visitor) const override;
    };

    class BaseMethod : non_copyable {
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
        std::string termType() { return "ConstructorBody"; };
        ConstructorBody() {};

        void addProperty(const std::string &propertyName);
        void addSuperArg(const std::string &variableName);

        std::vector<std::string> getProperties() const { return properties; }
        std::vector<std::string> getSuperArgs() const { return superArgs; }
    private:
        std::vector<std::string> properties;
        std::vector<std::string> superArgs;
    };

    class MethodDeclaration : public BaseMethod {
    public:
        std::string termType() { return "MethodDeclaration"; };
        // TODO: Remove this constructor
        MethodDeclaration(const std::string &name,
                          const std::string &return_class_name)
                : return_class_name(return_class_name) {
            this->name = name;
        }
        MethodDeclaration() {}

        void setBodyTerm(std::shared_ptr< MethodTerm > methodTerm) {
            this->treeHead = methodTerm;
        }

        std::shared_ptr< MethodTerm > getBodyTerm() const { return treeHead; }

        void setReturnClassName(const std::string &return_class_name);

        std::string getReturnClassName() const { return return_class_name; }

    private:
        std::string return_class_name;
        std::shared_ptr< MethodTerm > treeHead = nullptr;
    };

    class ClassDeclaration : non_copyable {
    public:
        ClassDeclaration() {};

        void setName(const std::string &name) { this->name = name; };
        void setParentName(const std::string &name) { this->parentName = name; };
        void addProperty(const std::string &className,
                         const std::string &propertyName) {
            // TODO: Fail if not unique.
            properties[propertyName] = className;
        };
        void addMethod(std::shared_ptr< MethodDeclaration > methodDeclaration) {
            methods.push_back(methodDeclaration);
        }

        void setConstructorBody(std::shared_ptr< ConstructorBody > constructor) {
            // TODO: Fail if already presence -
            // means constructor is defined twice.
            this->constructorBody = constructor;
        }

        std::shared_ptr< ConstructorBody > getConstructorBody() const {
            return std::move(constructorBody);
        }

        std::vector< std::shared_ptr< MethodDeclaration > > getMethods() const {
            return methods;
        }
        std::string getName() const { return name; };
        std::string getParentName() const { return parentName; };
        const std::map<std::string, std::string> getProperties() const {
            return std::move(properties);
        };

    private:
        std::string name, parentName;
        std::map<std::string, std::string> properties;
        std::vector< std::shared_ptr< MethodDeclaration > > methods;
        std::shared_ptr< ConstructorBody > constructorBody;
    };

    class ParsedContext {
    public:
        ParsedContext() {};
        void addClass(std::shared_ptr< ClassDeclaration > newClass) {
            classes.push_back(std::move(newClass));
        };
        std::vector< std::shared_ptr< ClassDeclaration > > getClasses() const {
            return std::move(classes);
        }
    private:
        std::vector< std::shared_ptr< ClassDeclaration > > classes;
    };
}

#endif //FJ_STRUCTURES_H
