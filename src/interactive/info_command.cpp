#include <iostream>

#include "interactive/info_command.h"
#include "interactive/mode.h"

#include "ast/context_builder.h"

namespace fj {
    int InfoCommand::execute(std::string arg) {
        for (auto &classPair : mode->getContext()->getClasses()) {
            if (classPair.first.t == arg) {
                printClass(classPair.second);
                return 0;
            }
        }

        std::cout << "info: Failed to find a class `" << arg
            << "'" << std::endl;
        return -1;
    }

    int InfoCommand::printClass(std::shared_ptr< ObjectClassBody > classBody) {
        std::cout << classBody->getClassName().t << " : "
            << classBody->getParentClass()->getClassName().t << " (";

        bool first = true;
        for (auto property : classBody->getProperties()) {
            if (first) {
                first = !first;
            } else {
                std::cout << ", ";
            }
            std::cout << property.second.t << " " << property.first.t;
        }

        std::cout << ")" << std::endl;
        return 0;
    }
}
