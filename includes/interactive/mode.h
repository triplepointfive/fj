#include <string>
#include <vector>

#include <readline/readline.h>
#include <readline/history.h>

namespace fj {
  class InteractiveMode {
    public:
      static int run();
      static InteractiveMode* instance;
      static char *command_generator(const char *text, int state);
      static char **fileman_completion(const char *, int, int);

    private:
      InteractiveMode();

      struct Command {
        std::string name;
        rl_icpfunc_t *func;
        std::string doc;
      };

      int iterate();
      void initialize_readline();

      int com_help(std::string arg);

      int execute_line(char *line);
      std::vector< Command > commands;
  };
}
