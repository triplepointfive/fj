#ifndef FJ_TERM_H
#define FJ_TERM_H

#include <iostream>
#include <vector>

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

typedef vector<Term *> Arguments;

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
    Access(Term *object, const string propertyName) {
        this->object = object;
        this->propertyName = propertyName;
    }

    string show() const override {
        return "(" + object->show() + ")." + propertyName;
    }

    string getPropertyName() const { return propertyName; }

    Term *getObject() const { return object; }

    void accept(TermVisitor &) override;

    virtual ~Access() { delete object; }

private:
    Term *object;
    string propertyName;
};

class Invocation : public Term {
public:
    Invocation(Term *object, string methodName, Arguments args) {
        this->object = object;
        this->methodName = methodName;
        this->args = args;
    }

    string show() const override;

    void accept(TermVisitor &) override;

    virtual ~Invocation() {
        delete object;
        for (auto &term : args) {
            delete term;
        }
    }

private:
    Term *object;
    string methodName;
    Arguments args;
};

class Constructor : public Term {
public:
    Constructor(string className, Arguments args) {
        this->className = className;
        this->args = args;
    }

    string show() const override;

    void accept(TermVisitor &) override;

    bool isValue() const override { return true; }

    virtual ~Constructor() {
        for (auto &term : args) {
            delete term;
        }
    }

private:
    string className;
    Arguments args;
};

class Coercion : public Term {
public:
    Coercion(string className, Term *object) {
        this->className = className;
        this->object = object;
    }

    string show() const override {
        return "(" + className + ") " + object->show();
    }

    void accept(TermVisitor &) override;

    virtual ~Coercion() {
        delete object;
    }

private:
    string className;
    Term *object;
};

#endif //FJ_TERM_H
