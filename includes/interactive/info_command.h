#ifndef FJ_INTERACTIVE_INFO_COMMAND_H
#define FJ_INTERACTIVE_INFO_COMMAND_H

#include <fstream>

#include "interactive/command.h"

namespace fj {
    class ObjectClassBody;

    class InfoCommand : public InteractiveCommand {
    public:
        InfoCommand(std::shared_ptr< InteractiveMode > mode,
                std::string name = "info")
            : InteractiveCommand(mode, name, "Show info about class or method")
            {}

        int execute(std::string arg) override;
    private:
        int printClass(std::shared_ptr< ObjectClassBody > classBody);
    };
}

#endif //FJ_INTERACTIVE_INFO_COMMAND_H
