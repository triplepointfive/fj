#include <iostream>

#include <stdio.h>

#include <sys/types.h>
#include <sys/file.h>
#include <sys/stat.h>
#include <sys/errno.h>

#include <strings.h>
#include <sstream>

#include "term.h"
#include "context.h"

#include "interactive/info_command.h"
#include "interactive/import_command.h"
#include "interactive/help_command.h"
#include "interactive/mode.h"

using namespace std;

namespace fj {
    using std::string;

    std::shared_ptr< InteractiveMode > InteractiveMode::instance = nullptr;

    char *dupstr(char *s) {
        char *r = (char *)malloc (strlen (s));
        strcpy (r, s);
        return (r);
    }

    /* Generator function for command completion.  STATE lets us know whether
       to start from scratch; without any state (i.e. STATE == 0), then we
       start at the top of the list. */
    char *InteractiveMode::command_generator(const char *text, int state) {
        static std::vector< std::shared_ptr < InteractiveCommand > >
            ::size_type i;

        if (state == 0) {
            i = 0;
        }

        /* Return the next name which partially matches from the command list. */
        const std::string name(text);
        while (i != instance->getCommands().size()) {
            auto command = instance->getCommands()[i];
            i++;
            if (command->getName().find(name) == 0) {
                auto comName = command->getName();
                char * writable = new char[comName.size() + 1];
                std::copy(comName.begin(), comName.end(), writable);
                writable[comName.size()] = '\0';
                return writable;
            }
        }

        /* If no names matched, then return NULL. */
        return nullptr;
    }

    char *InteractiveMode::info_generator(const char *text, int state) {
        static std::map<ClassName, std::shared_ptr< ObjectClassBody >>
            ::iterator i;
        static std::map<ClassName, std::shared_ptr< ObjectClassBody >> classes;

        if (state == 0) {
            classes = instance->getContext()->getClasses();
            i = classes.begin();
        }

        const std::string inputText(text);
        while (i != classes.end()) {
            auto commandName = i->first.t;
            i++;
            if (commandName.find(inputText) == 0) {
                char * writable = new char[commandName.size() + 1];
                std::copy(commandName.begin(), commandName.end(), writable);
                writable[commandName.size()] = '\0';
                return writable;
            }
        }

        return nullptr;
    }

    /* Tell the GNU Readline library how to complete.  We want to try to complete
       on command names if this is the first word in the line, or on filenames
       if not. */
    void InteractiveMode::initialize_readline() {
        /* Allow conditional parsing of the ~/.inputrc file. */
        rl_readline_name = "fj";

        /* Tell the completer that we want a crack first. */
        rl_attempted_completion_function = fj_completion;
    }

    /* Attempt to complete on the contents of TEXT.  START and END bound the
       region of rl_line_buffer that contains the word to complete.  TEXT is
       the word to complete.  We can use the entire contents of rl_line_buffer
       in case we want to do some simple parsing.  Return the array of matches,
       or NULL if there aren't any. */
    char **InteractiveMode::fj_completion(const char *text, int start, int) {
        /* If this word is at the start of the line, then it is a command
           to complete.  Otherwise it is the name of a file in the current
           directory. */
        if (start == 0) {
            return rl_completion_matches(text, command_generator);
        } else {
            std::stringstream stream(rl_line_buffer);
            std::string commandName;
            stream >> commandName;
            // TODO: Convert commands list into a mapping.
            if ( commandName == "info" ) {
                return rl_completion_matches(text, info_generator);
            }
        }

        return nullptr;
    }

    /* Strip whitespace from the start and end of STRING.  Return a pointer
       into STRING. */
    char *stripwhite(char *string) {
        char *s, *t;

        for (s = string; whitespace (*s); s++)
          ;

        if (*s == 0)
            return (s);

        t = s + strlen (s) - 1;
        while (t > s && whitespace (*t))
            t--;
        *++t = '\0';

        return s;
    }

    /* Execute a command line. */
    int InteractiveMode::execute_line(char *line) {
        /* Isolate the command word. */
        int i = 0;
        while (line[i] && whitespace(line[i]))
            i++;
        char *word = line + i;

        while (line[i] && !whitespace(line[i]))
            i++;

        if (line[i])
            line[i++] = '\0';

        for (auto command : commands) {
            if (word == command->getName()) {
                /* Get argument to command, if any. */
                while (whitespace (line[i]))
                    i++;

                word = line + i;

                /* Call the function. */
                return command->execute(std::string(word));
            }
        }

        fprintf (stderr, "%s: No such command for fj.\n", word);
        return -1;
    }

    InteractiveMode::InteractiveMode() {
        setContext(std::make_shared< Context >());
    }

    int InteractiveMode::run() {
        instance = std::make_shared< InteractiveMode >();

        instance->addCommand(std::make_shared< HelpCommand >(instance));
        instance->addCommand(std::make_shared< HelpCommand >(instance, "?"));
        instance->addCommand(std::make_shared< ImportCommand >(instance));
        instance->addCommand(std::make_shared< InfoCommand >(instance));

        int code = instance->iterate();
        instance = nullptr;
        return code;
    }

   int InteractiveMode::iterate() {
      char prompt [250];
      unsigned int commandNumber = 1;

      char *line, *s;

      initialize_readline();
      while(1) {
          sprintf(prompt, "%.3d > ", commandNumber);
          line = readline(prompt);
          commandNumber++;
          if (!line)
              break;

          s = stripwhite(line);

          if (*s) {
              add_history(s);
              execute_line(s);
          }

          free(line);
      }

      printf("Leaving fji.");
      return 0;
  }
}
