#include "lexer/lexer.h"
#include "lexer/parser_rules.h"
#include "lexer/parsing_structures.h"

namespace fj {
    bool parseContent(ParsedContext &context, std::string input, std::string source) {
        return pegtl::parse< fj::grammar, pegtl::nothing, fj::control >(
            input,
            source,
            context
        );
    }

    bool parseBody(MethodState &methodState, std::string input, std::string source) {
        return parse< fj::returned_statement, fj::build_method, fj::control>(
            input,
            source,
            methodState
        );
    }
}
