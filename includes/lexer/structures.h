#ifndef FJ_STRUCTURES_H
#define FJ_STRUCTURES_H

#include <string>
#include <iostream>
#include <map>
#include <vector>
#include <assert.h>
#include <memory>

using Properties = std::map<std::string, std::string>;
using Arguments = std::map<std::string, std::string>;
using Methods = std::map<std::string, std::string>;

class ParsedContext;

class MethodTerm {
public:
    MethodTerm(const std::string &name) : name(name) { }

    std::string getName() const;

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

    void addProperty(const std::string &propertyName);

    std::vector<std::string> getProperties() const { return properties; }

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

    void setBodyTerm(MethodTerm *methodTerm) { this->treeHead = methodTerm; }

    MethodTerm *getBodyTerm() const { return treeHead; }

    void setReturnClassName(const std::string &return_class_name);

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

    void setConstructorBody(std::shared_ptr< ConstructorBody > constructorBody) {
        // TODO: Fail if already presence - means constructor is defined twice.
        this->constructorBody = constructorBody;
    }

    std::shared_ptr< ConstructorBody > getConstructorBody() const {
        return std::move(constructorBody);
    }

    std::vector<MethodDeclaration*> getMethods() const { return methods; }
    std::string getName() const { return name; };
    std::string getParentName() const { return parentName; };
    const Properties *getProperties() const { return &properties; };

private:
    std::string name, parentName;
    Properties properties;
    std::vector<MethodDeclaration*> methods;
    std::shared_ptr< ConstructorBody > constructorBody;
};

class ParsedContext {
public:
    ParsedContext() {};
    void addClass(std::shared_ptr< ClassDeclaration > newClass) {
        classes.push_back(std::move(newClass));
    };
    std::vector< std::shared_ptr< ClassDeclaration > > getClasses() const {
        return classes;
    }
private:
    std::vector< std::shared_ptr< ClassDeclaration > > classes;
};

#endif //FJ_STRUCTURES_H
