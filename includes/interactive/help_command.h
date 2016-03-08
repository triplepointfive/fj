#ifndef FJ_INTERACTIVE_HELP_COMMAND_H
#define FJ_INTERACTIVE_HELP_COMMAND_H

#include "interactive/command.h"

namespace fj {
    class HelpCommand : public InteractiveCommand {
    public:
        HelpCommand(std::shared_ptr< InteractiveMode > mode, 
                std::string name = "help")
            : InteractiveCommand(mode, name, "Display this text")
            {}

        int execute(std::string arg) override;
    };
}

#endif //FJ_INTERACTIVE_HELP_COMMAND_H
