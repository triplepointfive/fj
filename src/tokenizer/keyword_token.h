#ifndef FJ_KEYWORD_TOKEN_H
#define FJ_KEYWORD_TOKEN_H

#include "token.h"

class KeywordToken : public Token {

};

class ClassToken : public KeywordToken {
};

class ExtendsToken : public KeywordToken {
};

class SuperToken : public KeywordToken {
};

class ReturnToken : public KeywordToken {
};

class NewToken : public KeywordToken {
};

class ThisToken : public KeywordToken {
};

#endif //FJ_KEYWORD_TOKEN_H
