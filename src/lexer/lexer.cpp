#include "lexer/lexer.h"
#include "lexer/parser_rules.h"

namespace fj {
    bool parseContent(ParsedContext &context, std::string input, std::string source) {
        return pegtl::parse< fj::grammar, pegtl::nothing, fj::control >(
            input,
            source,
            context
        );

    }
}
