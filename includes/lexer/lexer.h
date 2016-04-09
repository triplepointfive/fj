#ifndef FJ_LEXER_PARSER_H
#define FJ_LEXER_PARSER_H

#include "lexer/structures.h"
#include "lexer/lexer_term_visitor.h"
#include "lexer/parsing_structures.h"
#include <pegtl.hh>

namespace fj {
    // TODO: Declare new exception type, pegtl independent.
    bool parseContent(ParsedContext &context, std::string input, std::string source);

    bool parseBody(MethodState &methodState, std::string input, std::string source);
}

#endif //FJ_LEXER_PARSER_H
