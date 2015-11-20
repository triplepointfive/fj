#include <iostream>
#include <vector>

using namespace std;

namespace FJ {
    class Term {
    };

    typedef vector<Term *> Arguments;

    class Variable : public Term {
        Variable(const string name) {
            this->name = name;
        }

        string getName() const { return name; }

    private:
        string name;
    };

    class Access : public Term {
        Access(Term *object, const string propertyName) {
            this->object = object;
            this->propertyName = propertyName;
        }

        string getName() const { return propertyName; }

        ~Access() {
            delete object;
        }

    private:
        Term *object;
        string propertyName;
    };

    class Invocation : public Term {
        Invocation(Term *object, string methodName, Arguments args) {
            this->object = object;
            this->methodName = methodName;
            this->args = args;
        }

        ~Invocation() {
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
        Constructor(string className, Arguments args) {
            this->className = className;
            this->args = args;
        }

    private:
        string className;
        Arguments args;
    };
}

int main() {
    cout << "Hello, World!" << endl;
    return 0;
}
