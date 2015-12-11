#ifndef FJ_KEYWORD_TOKEN_H
#define FJ_KEYWORD_TOKEN_H

#include "token.h"

class KeywordToken : public Token {
public:
    KeywordToken(size_t &line, size_t &column, std::string token)
    : Token(line, column, token) {}
};

class ClassToken : public KeywordToken {
public:
    ClassToken(size_t &line, size_t &column, std::string token)
    : KeywordToken(line, column, token) {}
};

class ExtendsToken : public KeywordToken {
public:
    ExtendsToken(size_t &line, size_t &column, std::string token)
    : KeywordToken(line, column, token) {}
};

class SuperToken : public KeywordToken {
public:
    SuperToken(size_t &line, size_t &column, std::string token)
    : KeywordToken(line, column, token) {}
};

class ReturnToken : public KeywordToken {
public:
    ReturnToken(size_t &line, size_t &column, std::string token)
    : KeywordToken(line, column, token) {}
};

class NewToken : public KeywordToken {
public:
    NewToken(size_t &line, size_t &column, std::string token)
    : KeywordToken(line, column, token) {}
};

class ThisToken : public KeywordToken {
public:
    ThisToken(size_t &line, size_t &column, std::string token)
    : KeywordToken(line, column, token) {}
};

#endif //FJ_KEYWORD_TOKEN_H
