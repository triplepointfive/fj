#ifndef FJ_TOKEN_H
#define FJ_TOKEN_H

#include <string>
#include <iostream>
#include "glob.h"

class Token {
public:
    Token(size_t &line, size_t &column, std::string token) {
        this->line = line;
        this->column = column;
        this->token = token;
    }
    std::string show() const {
        std::cout << token;
        return std::to_string(line) + "," + std::to_string(column) + ": " + token;
    }

protected:
    size_t line, column;
    std::string token;
};

#endif //FJ_TOKEN_H
