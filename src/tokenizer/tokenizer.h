#ifndef FJ_TOKENIZER_H
#define FJ_TOKENIZER_H

#include <vector>
#include <string>

#include "keyword_token.h"

class Tokenizer {
public:
    Tokenizer(std::string fileName);

    std::vector<Token*> tokenize();

private:
    void releaseToken();
    char getNextChar();
    void addToAcc(char);

    std::vector<Token*> tokens;
    std::string content, acc;
    size_t currentLine, currentColumn;
    size_t positionInFile;
};

#endif //FJ_TOKENIZER_H
