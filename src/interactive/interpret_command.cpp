#include <iostream>

#include "interactive/interpret_command.h"
#include "interactive/mode.h"

#include "ast/class_table.h"
#include "context.h"

#include "lexer/lexer.h"
#include "lexer/structures.h"

#include "ast/lexer_term_builder.h"
#include "eval_term_visitor.h"

namespace fj {
    int InterpretCommand::execute(std::string arg) {
        std::cout << arg << std::endl;
        MethodState methodState;

        // TODO: Debug only;
        /* std::cout << arg << std::endl; */

        bool status;
        try {
            status = parseBody(methodState, arg, "input");

            if (!status) {
                std::cout << "Parsing failed for now reason!" << std::endl;
                return -1;
            }

            auto term = methodState.methodDeclaration->getBodyTerm();
            std::cout << "parsed term: " << term->inspect() << std::endl;

            LexerTermBuilder builder;
            term->accept(&builder);
            auto astTerm = builder.getTerm();
            std::cout << "built tree: " << astTerm->show() << std::endl;

            EvalTermVisitor evaluator(Context(mode->getClassTable()));
            astTerm->accept(evaluator);

            auto val = evaluator.getCalculatedValue();
            std::cout << val->show() << std::endl;
        }
        catch (const pegtl::parse_error& e) {
            std::cout << "interpret: " << e.what() << std::endl;
            return -1;
        }

        /* std::cout << status << std::endl; */
        return 0;
    }
}
