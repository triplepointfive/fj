#include "tokenizer.h"

#include <fstream>
#include <boost/algorithm/string.hpp>

Tokenizer::Tokenizer(std::string fileName) {
    std::ifstream t(fileName);
    this->content = std::string(std::istreambuf_iterator<char>(t),
                                std::istreambuf_iterator<char>());

    this->currentColumn = 1;
    this->currentLine = 1;
    this->positionInFile = 0;
}

void Tokenizer::releaseToken() {
    if (acc.size() != 0) {
        acc = "";
    }
}

std::vector<Token*> Tokenizer::tokenize() {
    while (true) {
        char nextChat = getNextChar();
        switch (nextChat) {
            case 0:
                releaseToken();
                break;
            case '\n':
                currentLine++;
                currentColumn = 1;
            case '\t':
            case ' ':
                releaseToken();
                break;
            case '}':
            case '{':
                releaseToken();
                addToAcc(nextChat);
                releaseToken();
                break;
            default:

        }
        break;
    }
}

void Tokenizer::addToAcc(char c) {
    acc += c;
}

char Tokenizer::getNextChar() {
    char c = content[positionInFile];
    positionInFile++;
    currentColumn++;
    return c;
}
