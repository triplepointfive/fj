#include <iostream>
#include <vector>

#include "term.h"
#include "tokenizer/tokenizer.h"

using namespace std;

int main() {
    Tokenizer tokenizer("/home/ilya/Projects/fj/examples/simple_class.java");
    std::vector<Token*> tokens = tokenizer.tokenize();
    for (auto elem : tokens) {
        std::cout << elem->show() << std::endl;
    }
    return 0;
}
