#ifndef FJ_BRACKET_TOKEN_H
#define FJ_BRACKET_TOKEN_H

#include "token.h"

class CurlyOpenBracketToken : public Token {
public:
    CurlyOpenBracketToken(size_t &line, size_t &column, std::string token)
            : Token(line, column, token) {}
};

class CurlyCloseBracketToken : public Token {
public:
    CurlyCloseBracketToken(size_t &line, size_t &column, std::string token)
    : Token(line, column, token) {}
};

class OpenParenthesesToken : public Token {
public:
    OpenParenthesesToken(size_t &line, size_t &column, std::string token)
    : Token(line, column, token) {}
};

class CloseParenthesesToken : public Token {
public:
    CloseParenthesesToken(size_t &line, size_t &column, std::string token)
    : Token(line, column, token) {}
};

#endif //FJ_BRACKET_TOKEN_H
