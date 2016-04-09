#ifndef FJ_INTERACTIVE_INTERPRET_COMMAND_H
#define FJ_INTERACTIVE_INTERPRET_COMMAND_H

#include <fstream>

#include "interactive/command.h"

namespace fj {
    class ObjectClassBody;

    class InterpretCommand : public InteractiveCommand {
    public:
        InterpretCommand(std::shared_ptr< InteractiveMode > mode,
                std::string name = ">")
            : InteractiveCommand(mode, name, "Interpret input line")
            {}

        int execute(std::string arg) override;
    private:
    };
}

#endif //FJ_INTERACTIVE_INTERPRET_COMMAND_H
