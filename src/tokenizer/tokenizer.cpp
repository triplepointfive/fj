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
        Token *newToken = new Token(currentLine, currentColumn, acc);
        tokens.push_back(newToken);
        acc = "";
        currentColumn = positionInFile;
    }
}

std::vector<Token*> Tokenizer::tokenize() {
    while (true) {
        char nextChat = getNextChar();
        switch (nextChat) {
            case 0:
                releaseToken();
                return tokens;
            case '\n':
                currentLine++;
                currentColumn = 1;
            case ',':
            case ';':
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
                addToAcc(nextChat);
                break;
        }
    }
}

void Tokenizer::addToAcc(char c) {
    acc += c;
}

char Tokenizer::getNextChar() {
    return content[positionInFile++];
}
