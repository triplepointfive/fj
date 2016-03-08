#include <iostream>

#include "interactive/help_command.h"
#include "interactive/mode.h"

namespace fj {
    int HelpCommand::execute(std::string arg) {
        int printed = 0;

        for (auto &command : mode->getCommands()) {
            if (arg == "" || command->getName() == arg) {
                std::cout << command->getName() << "\t\t"
                    << command->getDoc() << "." << std::endl;
                printed++;
            }
        }

        if (!printed) {
            std::cout << "No commands match `" << arg
                << "'.  Possibilties are:\n";

            for ( auto command : mode->getCommands() ) {
                /* Print in six columns. */
                if (printed == 6) {
                    printed = 0;
                    std::cout << "\n";
                }

                std::cout << command->getName() << "\t";
                printed++;
            }

            if (printed) {
                std::cout << std::endl;
            }
        }
        return 0;
    }
}
