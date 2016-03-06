#include <iostream>
#include <vector>
#include <string>

#include <stdio.h>

#include <sys/types.h>
#include <sys/file.h>
#include <sys/stat.h>
#include <sys/errno.h>

#include <readline/readline.h>
#include <readline/history.h>
#include <strings.h>

#include "term.h"

using namespace std;

int com_help PARAMS((char *));

struct Command {
  char *name;
  rl_icpfunc_t *func;
  char *doc;
};

Command commands[] = {
  { "help", com_help, "Display this text" },
  { nullptr, nullptr, nullptr }
};

char *dupstr(char *s) {
  char *r = (char *)malloc (strlen (s));
  strcpy (r, s);
  return (r);
}

Command *find_command(char *name) {
  int i;

  for (i = 0; commands[i].name; i++)
    if (strcmp (name, commands[i].name) == 0)
      return (&commands[i]);

  return nullptr;
}

char *command_generator PARAMS((const char *, int));
char **fileman_completion PARAMS((const char *, int, int));

/* Tell the GNU Readline library how to complete.  We want to try to complete
   on command names if this is the first word in the line, or on filenames
   if not. */
void initialize_readline() {
  /* Allow conditional parsing of the ~/.inputrc file. */
  rl_readline_name = "FileMan";

  /* Tell the completer that we want a crack first. */
  rl_attempted_completion_function = fileman_completion;
}

/* Attempt to complete on the contents of TEXT.  START and END bound the
   region of rl_line_buffer that contains the word to complete.  TEXT is
   the word to complete.  We can use the entire contents of rl_line_buffer
   in case we want to do some simple parsing.  Return the array of matches,
   or NULL if there aren't any. */
char ** fileman_completion (const char *text, int start, int end) {
  char **matches = nullptr;

  /* If this word is at the start of the line, then it is a command
     to complete.  Otherwise it is the name of a file in the current
     directory. */
  if (start == 0)
    matches = rl_completion_matches (text, command_generator);

  return (matches);
}

/* Generator function for command completion.  STATE lets us know whether
   to start from scratch; without any state (i.e. STATE == 0), then we
   start at the top of the list. */
char *command_generator(const char *text, int state) {
    static int list_index, len;
    char *name;

    /* If this is a new word to complete, initialize now.  This includes
       saving the length of TEXT for efficiency, and initializing the index
       variable to 0. */
    if (!state) {
        list_index = 0;
        len = strlen (text);
    }

    /* Return the next name which partially matches from the command list. */
    while (name = commands[list_index].name) {
        list_index++;

        if (strncmp (name, text, len) == 0)
          return (dupstr(name));
    }

    /* If no names matched, then return NULL. */
    return nullptr;
}

int com_help(char *arg) {
    int i;
    int printed = 0;

    for (i = 0; commands[i].name; i++) {
        if (!*arg || (strcmp (arg, commands[i].name) == 0)) {
            printf ("%s\t\t%s.\n", commands[i].name, commands[i].doc);
            printed++;
        }
    }

    if (!printed) {
        printf ("No commands match `%s'.  Possibilties are:\n", arg);

        for (i = 0; commands[i].name; i++) {
            /* Print in six columns. */
            if (printed == 6) {
                printed = 0;
                printf ("\n");
            }

            printf ("%s\t", commands[i].name);
            printed++;
        }

        if (printed)
            printf ("\n");
    }
    return 0;
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
int execute_line(char *line) {
  register int i;
  Command *command;
  char *word;

  /* Isolate the command word. */
  i = 0;
  while (line[i] && whitespace (line[i]))
    i++;
  word = line + i;

  while (line[i] && !whitespace (line[i]))
    i++;

  if (line[i])
    line[i++] = '\0';

  command = find_command (word);

  if (!command)
    {
      fprintf (stderr, "%s: No such command for FileMan.\n", word);
      return (-1);
    }

  /* Get argument to command, if any. */
  while (whitespace (line[i]))
    i++;

  word = line + i;

  /* Call the function. */
  return ((*(command->func)) (word));
}

void interactive() {
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
}

int main() {
    interactive();
    return 0;
}
