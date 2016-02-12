#ifndef FJ_STRUCTURES_H
#define FJ_STRUCTURES_H

#include <string>
#include <iostream>
#include <map>
#include <vector>
#include <assert.h>
#include <memory>

namespace fj {
    using Arguments = std::map<std::string, std::string>;

    // TODO: Move to somewhere.
    class non_copyable {
    protected:
        non_copyable() = default;
        ~non_copyable() = default;

        non_copyable(non_copyable const &) = delete;
        void operator=(non_copyable const &x) = delete;
    };

    class ParsedContext;

    class MethodTerm {
    public:
        MethodTerm(const std::string &name) : name(name) { }
        MethodTerm() { }
        void setName(std::string name) { this->name = name; }
        std::string getName() const;
        virtual ~MethodTerm() {}

        // TODO: Debug only
        virtual std::string termType() = 0;
        virtual std::string inspect() = 0;
    protected:
        std::string name;
    };

    class PropertyTerm : public MethodTerm {
    public:
        std::string termType() { return "PropertyTerm"; };
        std::string inspect() { return "." + name; };
        PropertyTerm(std::string propertyName) : MethodTerm(propertyName) {}
    };

    class VariableTerm : public MethodTerm {
    public:
        std::string termType() { return "VariableTerm"; };
        std::string inspect() { return name; };
        VariableTerm(std::string variableName) : MethodTerm(variableName) {}
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
        std::string inspect() { return objectName + "." + name; };
        void setObjectName(const std::string &objectName) {
            this->objectName = objectName;
        }
        std::string getObjectName() const { return objectName; }
    private:
        std::string objectName;
    };

    class Initiation : public ArgumentTerm {
    public:
        std::string termType() { return "Initiation"; };
        std::string inspect() { return "new " + name; };
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

        void setConstructorBody(std::shared_ptr< ConstructorBody > constructorBody) {
            // TODO: Fail if already presence - means constructor is defined twice.
            this->constructorBody = constructorBody;
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

    struct ClassState {
        std::shared_ptr< ClassDeclaration > classDeclaration =
            std::make_shared< ClassDeclaration >();

        void success(ParsedContext & context);
    };

    struct ConstructorState {
        std::shared_ptr< ConstructorBody > constructorBody =
            std::make_shared< ConstructorBody >();

        void success(ClassState & classState);
    };

    struct MethodState {
        std::shared_ptr< MethodDeclaration > methodDeclaration =
            std::make_shared< MethodDeclaration >();

        void success(ClassState & classState);
    };

    struct ArguableTermState;
    struct MethodTermState;
    struct MethodInvocationState;
    struct InitiationState;
    struct TypeCastingState;

    struct BaseMethodTermState {
        virtual std::shared_ptr< MethodTerm > getTerm() = 0;

        // TODO: Seems redundant
        void success(MethodTermState & methodTermState);

        void success(MethodState & methodState);
        void success(MethodInvocationState & methodInvocationState);
        void success(InitiationState & initiationState);
        void success(TypeCastingState & typeCastingState);

        virtual ~BaseMethodTermState() {}
    };

    struct MethodTermState : BaseMethodTermState {
        std::shared_ptr< MethodTerm > methodTerm = nullptr;

        std::shared_ptr< MethodTerm > getTerm() override {
            return methodTerm;
        };
    };

    struct ArguableTermState : BaseMethodTermState {
        virtual void addArg(std::shared_ptr< MethodTerm >) = 0;
        virtual ~ArguableTermState() {}
    };

    struct InitiationState : ArguableTermState {
        std::shared_ptr< Initiation > initiation =
            std::make_shared<Initiation>();

        void addArg(std::shared_ptr< MethodTerm > term) {
            initiation->addArg(term);
        }

        std::shared_ptr< MethodTerm > getTerm() override {
            return initiation;
        };
    };

    struct MethodInvocationState : ArguableTermState {
        std::shared_ptr< MethodInvocation > methodInvocation =
            std::make_shared< MethodInvocation >();

        void addArg(std::shared_ptr< MethodTerm > term) {
            methodInvocation->addArg(term);
        }

        std::shared_ptr< MethodTerm > getTerm() override {
            return methodInvocation;
        };
    };

    struct TypeCastingState : ArguableTermState {
        std::shared_ptr< TypeCastingTerm > typeCastingTerm =
            std::make_shared< TypeCastingTerm >();

        void addArg(std::shared_ptr< MethodTerm > term) {
            typeCastingTerm->setTerm(term);
        }

        std::shared_ptr< MethodTerm > getTerm() override {
            return typeCastingTerm;
        };
    };

    struct PairState {
        std::string key;
        std::string val;

        void success(ClassState & classState);
    };

    struct MethodArgState {
        std::string name;
        std::string returnedClassName;

        void success(ConstructorState & constructorState);
        void success(MethodState & methodState);
    };
}

#endif //FJ_STRUCTURES_H
