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

    class InteractiveMode {
    public:
        static int run();
        static std::shared_ptr< InteractiveMode > instance;
        static char *command_generator(const char *text, int state);
        static char **fileman_completion(const char *, int, int);

        std::vector< std::shared_ptr< InteractiveCommand > > getCommands() {
            return commands;
        }
        void addCommand(std::shared_ptr< InteractiveCommand > command) {
            commands.push_back(command);
        }
    private:
        int iterate();
        void initialize_readline();

        int com_help(std::string arg);

        int execute_line(char *line);
        std::vector< std::shared_ptr< InteractiveCommand > > commands;
    };
}

#endif //FJ_INTERACTIVE_MODE_H
