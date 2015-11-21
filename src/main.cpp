#include <iostream>
#include <vector>

#include "term.h"

using namespace std;

int main() {
    Arguments args;

    args.push_back(new Variable("newfst"));

    Variable *thisObj = new Variable("this");
    args.push_back(new Access(thisObj, "snd"));

    Constructor *c = new Constructor("Pair", args);
    cout << c->show();
    delete c;
    return 0;
}
