#include <iostream>
#include <map>

using namespace std;

class Term;

class Variable;

typedef map<Variable *, Type> Context;

const Type objectType = "Object";

Type getTypeFromContext(Context ctx, int index) {

}

Type typeOf(Context ctx, Term *term) {
    return objectType;
}
