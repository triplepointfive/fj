#ifndef FJ_INTERACTIVE_IMPORT_COMMAND_H
#define FJ_INTERACTIVE_IMPORT_COMMAND_H

#include <fstream>

#include "interactive/command.h"

namespace fj {
    class ImportCommand : public InteractiveCommand {
    public:
        ImportCommand(std::shared_ptr< InteractiveMode > mode,
                std::string name = "import")
            : InteractiveCommand(mode, name, "Import java file")
            {}

        int execute(std::string arg) override;
    private:
        int import(std::string fileName);
    };
}

#endif //FJ_INTERACTIVE_IMPORT_COMMAND_H
