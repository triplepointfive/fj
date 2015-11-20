#include <iostream>
#include <vector>

using namespace std;


namespace FJ {
    class Term {
    public:
        virtual ~Term() { }

        virtual string Show() = 0;
    };

    typedef vector<Term *> Arguments;

    string show(Arguments args) {
        string acc = "";
        bool firstElement = true;

        for (auto &element : args) {
            if (firstElement) {
                firstElement = false;
            } else {
                acc += ", ";
            }
            acc += element->Show();
        }
        return acc;
    }

    class Variable : public Term {
    public:
        Variable(const string name) {
            this->name = name;
        }

        string getName() const { return name; }

        string Show() override { return name; }

    private:
        string name;
    };

    class Access : public Term {
    public:
        Access(Term *object, const string propertyName) {
            this->object = object;
            this->propertyName = propertyName;
        }

        string getName() const { return propertyName; }

        string Show() override {
            return "(" + object->Show() + ")." + propertyName;
        }

        virtual ~Access() {
            delete object;
        }

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

        string Show() override {
            return object->Show() + "." + methodName + "(" + show(args) + ")";
        }

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

        string Show() override {
            return "new " + className + "(" + show(args) + ")";
        }

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

        string Show() override {
            return "(" + className + ") " + object->Show();
        }

        virtual ~Coercion() {
            delete object;
        }

    private:
        string className;
        Term *object;
    };
}

int main() {
    FJ::Arguments args;

    args.push_back(new FJ::Variable("newfst"));

    FJ::Variable *thisObj = new FJ::Variable("this");
    args.push_back(new FJ::Access(thisObj, "snd"));

    FJ::Constructor *c = new FJ::Constructor("Pair", args);
    cout << c->Show();
    delete c;
    return 0;
}
