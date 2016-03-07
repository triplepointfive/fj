#include <string>
#include <vector>
#include <memory>

#include <readline/readline.h>
#include <readline/history.h>

namespace fj {
    class InteractiveCommand;

    class InteractiveMode {
    public:
        InteractiveMode();

        static int run();
        static std::shared_ptr< InteractiveMode > instance;
        static char *command_generator(const char *text, int state);
        static char **fileman_completion(const char *, int, int);

        std::vector< std::shared_ptr< InteractiveCommand > > getCommands() {
            return std::move(commands);
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

    class InteractiveCommand {
    public:
        InteractiveCommand(std::shared_ptr< InteractiveMode > mode,
                std::string name, std::string doc)
            : mode(mode)
            , name(name)
            , doc(doc)
            {}

        virtual ~InteractiveCommand() = default;

        virtual int execute(std::string) = 0;
        const std::string &getName() const { return name; };
        const std::string &getDoc()  const { return doc;  };
    protected:
        std::shared_ptr< InteractiveMode > mode{ nullptr };
        std::string name;
        std::string doc;
    };

    class HelpCommand : public InteractiveCommand {
    public:
        HelpCommand(std::shared_ptr< InteractiveMode > mode)
            : InteractiveCommand(mode, "help", "Display this text")
            {}

        int execute(std::string arg) override;
    };
}
