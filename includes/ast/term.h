#ifndef FJ_TERM_H
#define FJ_TERM_H

#include <assert.h>
#include <iostream>
#include <map>
#include <vector>
#include <memory>

#include "base_types.h"
#include "type.h"

namespace fj {
    using std::string;
    using std::map;

    class TermVisitor;

    // TODO: Disable copying.
    class Term {
    public:
        virtual ~Term() { }

        virtual string show() const = 0;

        virtual void accept(TermVisitor &) = 0;

        virtual bool isValue() const { return false; }
    };

    using TermPtr = std::shared_ptr< Term >;

    using MethodArguments = map<PropertyName, TermPtr>;

    class Variable : public Term {
    public:
        Variable(const PropertyName name) {
            this->name = name;
        }

        string show() const override { return name.t; }

        PropertyName getName() const { return name; }
        void accept(TermVisitor &) override;

    private:
        PropertyName name;
    };

    class Access : public Term {
    public:
        Access(TermPtr object, PropertyName propertyName) {
            this->object = object;
            this->propertyName = propertyName;
        }

        string show() const override {
            return "(" + object->show() + ")." + propertyName.t;
        }

        PropertyName getPropertyName() const { return propertyName; }

        TermPtr getObject() const { return std::move(object); }

        void accept(TermVisitor &) override;

    private:
        TermPtr object;
        PropertyName propertyName;
    };

    class Invocation : public Term {
    public:
        Invocation(TermPtr object, MethodName methodName, MethodArguments args) {
            this->object = object;
            this->methodName = methodName;
            this->args = args;
        }

        TermPtr getObject() const { return std::move(object); }

        MethodArguments getArgs() const { return std::move(args); }

        MethodName getMethodName() const { return methodName; }

        string show() const override;

        void accept(TermVisitor &) override;

    private:
        TermPtr object;
        MethodName methodName;
        MethodArguments args;
    };

    class Constructor : public Term {
    public:
        Constructor(ClassName className, MethodArguments args) {
            this->className = className;
            this->args = args;
        }

        string show() const override;

        void accept(TermVisitor &) override;

        bool isValue() const override { return true; }

        ClassName getClassName() const { return className; }

        MethodArguments getArgs() const { return std::move(args); }

        TermPtr getAttribute(PropertyName propertyName) {
            // Accessing no-existing property, it is type checker's job.
            assert(args.count(propertyName) != 0);
            return std::move(args.find(propertyName)->second);
        }

    private:
        ClassName className;
        MethodArguments args;
    };

    class Coercion : public Term {
    public:
        Coercion(ClassName className, TermPtr object) {
            this->className = className;
            this->object = object;
        }

        string show() const override {
            return "(" + className.t + ") " + object->show();
        }

        TermPtr getObject() const { return std::move(object); }

        ClassName getClassName() const { return className; }

        void accept(TermVisitor &) override;

    private:
        ClassName className;
        TermPtr object;
    };

}

#endif //FJ_TERM_H
