#ifndef FJ_INTERACTIVE_MODE_H
#define FJ_INTERACTIVE_MODE_H

#include <iostream>

#include <string>
#include <vector>
#include <memory>

#include <readline/readline.h>
#include <readline/history.h>

namespace fj {
    class InteractiveCommand;
    class ClassTable;

    class InteractiveMode {
    public:
        InteractiveMode();
        static int run();
        static std::shared_ptr< InteractiveMode > instance;
        static char *command_generator(const char *text, int state);
        static char *info_generator(const char *text, int state);
        static char **fj_completion(const char *, int, int);

        std::vector< std::shared_ptr< InteractiveCommand > > getCommands() {
            return commands;
        }
        void addCommand(std::shared_ptr< InteractiveCommand > command) {
            commands.push_back(command);
        }
        std::shared_ptr< ClassTable > getClassTable() {
            return this->class_table;
        }
        void setClassTable(std::shared_ptr< ClassTable > class_table) {
            this->class_table = class_table;
        }

    private:
        int iterate();
        void initialize_readline();

        int com_help(std::string arg);

        int execute_line(char *line);
        std::vector< std::shared_ptr< InteractiveCommand > > commands;
        std::shared_ptr< ClassTable > class_table{ nullptr };
    };
}

#endif //FJ_INTERACTIVE_MODE_H
