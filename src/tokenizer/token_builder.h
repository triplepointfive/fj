#ifndef FJ_TOKEN_BUILDER_H
#define FJ_TOKEN_BUILDER_H

#include "token.h"
#include "keyword_token.h"
#include "bracket_token.h"

class TokenBuilder {
public:
    Token *build(size_t line, size_t column, std::string tokenStr);

private:
    bool keywordClass();
    bool keywordExtends();
    bool keywordSuper();
    bool keywordReturn();
    bool keywordNew();
    bool keywordThis();

    bool curlyOpenBracket();
    bool curlyCloseBracket();

    bool openParentheses();
    bool closeParentheses();

    size_t line, column;
    std::string tokenStr;
};

#endif //FJ_TOKEN_BUILDER_H
