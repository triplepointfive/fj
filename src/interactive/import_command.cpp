#include <iostream>

#include "interactive/import_command.h"
#include "interactive/mode.h"

#include "lexer/lexer.h"
#include "ast/context_builder.h"
#include "ast/class_table.h"

namespace fj {
    int ImportCommand::execute(std::string arg) {
        if (arg == "") {
            std::cout << "Syntax: import FILENAME" << std::endl;
            return -1;
        }
        if (!std::ifstream(arg).good()) {
            std::cout << "import: Failed to find or read file `"
                << arg << "'" << std::endl;
            return -1;
        }
        return import(arg);
    }

    int ImportCommand::import(std::string fileName) {
        std::ifstream stream(fileName);
        try {
            ParsedContext parsedContext;
            const std::string input((std::istreambuf_iterator< char >(stream) ),
                       (std::istreambuf_iterator< char >()));
            bool status = parseContent(parsedContext, input, fileName);

            if (!status) {
                std::cout << "import: Hit unknown parse error" << std::endl;
                return -2;
            }

            ContextBuilder builder;
            builder.buildAST(parsedContext);
            mode->setClassTable(builder.getClassTable());
        }
        catch (const pegtl::parse_error& e) {
            std::cout << "import: " << e.what() << std::endl;
        }
        return 0;
    }
}
