#ifndef FJ_TERM_H
#define FJ_TERM_H

#include <assert.h>
#include <iostream>
#include <map>
#include <vector>

#include "base_types.h"
#include "type.h"

using namespace std;

class TermVisitor;

class Term {
public:
    virtual ~Term() { }

    virtual string show() const = 0;

    virtual void accept(TermVisitor &) = 0;

    virtual bool isValue() const { return false; }
};

typedef map<string, Term *> Arguments;

class Variable : public Term {
public:
    Variable(const string name) {
        this->name = name;
    }

    string show() const override { return name; }

    void accept(TermVisitor &) override;

private:
    string name;
};

class Access : public Term {
public:
    Access(Term *object, PropertyName propertyName) {
        this->object = object;
        this->propertyName = propertyName;
    }

    string show() const override {
        return "(" + object->show() + ")." + propertyName.t;
    }

    PropertyName getPropertyName() const { return propertyName; }

    Term *getObject() const { return object; }

    void accept(TermVisitor &) override;

    virtual ~Access() { delete object; }

private:
    Term *object;
    PropertyName propertyName;
};

class Invocation : public Term {
public:
    Invocation(Term *object, MethodName methodName, Arguments args) {
        this->object = object;
        this->methodName = methodName;
        this->args = args;
    }

    Term *getObject() const { return object; }

    Arguments getArgs() const { return args; }

    MethodName getMethodName() const { return methodName; }

    string show() const override;

    void accept(TermVisitor &) override;

    virtual ~Invocation() {
        delete object;
        for (auto &pair : args) {
            delete pair.second;
        }
    }

private:
    Term *object;
    MethodName methodName;
    Arguments args;
};

class Constructor : public Term {
public:
    Constructor(ClassName, Arguments args) {
        this->className = className;
        this->args = args;
    }

    string show() const override;

    void accept(TermVisitor &) override;

    bool isValue() const override { return true; }

    ClassName getClassName() const { return className; }

    Arguments getArgs() const { return args; }

    Term *getAttribute(string propertyName) {
        // Accessing no-existing property, it is type checker's job.
        assert(args.count(propertyName) != 0);
        return args.find(propertyName)->second;
    }

    virtual ~Constructor() {
        for (auto &pair : args) {
            delete pair.second;
        }
    }

private:
    ClassName className;
    Arguments args;
};

class Coercion : public Term {
public:
    Coercion(ClassName className, Term *object) {
        this->className = className;
        this->object = object;
    }

    string show() const override {
        return "(" + className.t + ") " + object->show();
    }

    Term *getObject() const { return object; }

    ClassName getClassName() const { return className; }

    void accept(TermVisitor &) override;

    virtual ~Coercion() {
        delete object;
    }

private:
    ClassName className;
    Term *object;
};

#endif //FJ_TERM_H
