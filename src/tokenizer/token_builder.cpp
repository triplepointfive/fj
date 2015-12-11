#include "token_builder.h"

Token *TokenBuilder::build(size_t line, size_t column, std::string tokenStr) {
    this->line = line;
    this->column = column;
    this->tokenStr = tokenStr;

    if (keywordClass()) {
    } else if (keywordExtends()) {
        return new ExtendsToken(line, column, tokenStr);
    } else if (keywordNew()) {
        return new NewToken(line, column, tokenStr);
    } else if (keywordReturn()) {
        return new ReturnToken(line, column, tokenStr);
    } else if (keywordSuper()) {
        return new SuperToken(line, column, tokenStr);
    } else if (keywordThis()) {
        return new ThisToken(line, column, tokenStr);
    } else if (curlyOpenBracket()) {
        return new CurlyOpenBracketToken(line, column, tokenStr);
    } else if (curlyCloseBracket()) {
        return new CurlyOpenBracketToken(line, column, tokenStr);
    } else if (openParentheses()) {
        return new OpenParenthesesToken(line, column, tokenStr);
    } else if (closeParentheses()) {
        return new CloseParenthesesToken(line, column, tokenStr);
    } else {
        return new Token(line, column, tokenStr);
    }
}

bool TokenBuilder::keywordClass() {
    return tokenStr == "class";
}

bool TokenBuilder::keywordExtends() {
    return tokenStr == "extends";
}

bool TokenBuilder::keywordSuper() {
    return tokenStr == "super";
}

bool TokenBuilder::keywordReturn() {
    return tokenStr == "return";
}

bool TokenBuilder::keywordNew() {
    return tokenStr == "new";
}

bool TokenBuilder::keywordThis() {
    return tokenStr == "this";
}

bool TokenBuilder::curlyOpenBracket() {
    return tokenStr == "{";
}

bool TokenBuilder::curlyCloseBracket() {
    return tokenStr == "}";
}

bool TokenBuilder::openParentheses() {
    return tokenStr == "(";
}

bool TokenBuilder::closeParentheses() {
    return tokenStr == ")";
}
