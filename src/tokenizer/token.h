#ifndef FJ_TOKEN_H
#define FJ_TOKEN_H

#include "glob.h"

class Token {
    Token(size_t &line, size_t &column) {
        this->line = line;
        this->column = column;
    }
private:
    size_t line, column;
};

#endif //FJ_TOKEN_H
