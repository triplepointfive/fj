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
    class Context;

    class InteractiveMode {
    public:
        static int run();
        static std::shared_ptr< InteractiveMode > instance;
        static char *command_generator(const char *text, int state);
        static char **fj_completion(const char *, int, int);

        std::vector< std::shared_ptr< InteractiveCommand > > getCommands() {
            return commands;
        }
        void addCommand(std::shared_ptr< InteractiveCommand > command) {
            commands.push_back(command);
        }
        void setContext(std::shared_ptr< Context > context) {
            this->context = context;
        }

    private:
        int iterate();
        void initialize_readline();

        int com_help(std::string arg);

        int execute_line(char *line);
        std::vector< std::shared_ptr< InteractiveCommand > > commands;
        std::shared_ptr< Context > context{ nullptr };
    };
}

#endif //FJ_INTERACTIVE_MODE_H
