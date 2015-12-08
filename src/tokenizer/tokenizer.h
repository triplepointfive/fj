#ifndef FJ_TOKENIZER_H
#define FJ_TOKENIZER_H

#include <vector>
#include <string>

#include "keyword_token.h"

class Tokenizer {
public:
    Tokenizer(std::string fileName);

    std::vector<std::string> tokenize();

private:
    std::string getNextToken();

    std::string content;
};

#endif //FJ_TOKENIZER_H
