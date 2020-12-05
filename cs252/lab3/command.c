/*
 * CS252: Shell project
 *
 * DO NOT PUT THIS PROJECT IN A PUBLIC REPOSITORY LIKE GIT. IF YOU WANT
 * TO MAKE IT PUBLICALLY AVAILABLE YOU NEED TO REMOVE ANY SKELETON CODE
 * AND REWRITE YOUR PROJECT SO IT IMPLEMENTS FUNCTIONALITY DIFFERENT THAN
 * WHAT IS SPECIFIED IN THE HANDOUT. WE OFTEN REUSE PART OF THE PROJECTS FROM
 * SEMESTER TO SEMESTER AND PUTTING YOUR CODE IN A PUBLIC REPOSITORY
 * MAY FACILITATE ACADEMIC DISHONESTY.
 */

#include "command.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <errno.h>

#include "shell.h"

#define FILE_NOT_FOUND (-1)
#define MAX_PID_LEN (20)

extern char ** environ;
extern char ** g_history;
extern char * g_whole_command;
extern int g_whole_command_size;
extern int g_history_length;
extern int g_history_index;
int g_last_background_command = 0;
int g_last_background_pid = 0;
int g_last_return_code = 0;
char * g_last_argument = NULL;
char * g_last_command = NULL;

/*
 *  Initialize a command_t.
 */

void create_command(command_t *command) {
  command->single_commands = NULL;

  command->out_file = NULL;
  command->in_file = NULL;
  command->err_file = NULL;

  command->append_out = false;
  command->append_err = false;
  command->background = false;

  command->num_single_commands = 0;
} /* create_command() */

/*
 *  Insert a single command into the list of single commands in a command_t.
 */

void insert_single_command(command_t *command, single_command_t *simp) {
  if (simp == NULL) {
    return;
  }

  command->num_single_commands++;
  int new_size = (command->num_single_commands) * sizeof(single_command_t *);
  command->single_commands = (single_command_t **)
                              realloc(command->single_commands, new_size);
  command->single_commands[command->num_single_commands - 1] = simp;
} /* insert_single_command() */

/*
 * Insert a single command into g_history by dynamically reallocating the
 * command history list.
 */

void insert_into_history(char * last_command) {
  if (last_command != NULL) {
    g_history_length++;
    g_history = (char **) realloc(g_history, sizeof(char *) *
        g_history_length);
    g_history[g_history_length - 1] = strdup(last_command);
  }

  g_history_index = g_history_length;
} /* insert_into_history() */

/*
 * Free a command and its contents.
 */

void free_command(command_t *command) {
  for (int i = 0; i < command->num_single_commands; i++) {
    free_single_command(command->single_commands[i]);
    command->single_commands[i] = NULL;
  }

  free(command->single_commands);
  command->single_commands = NULL;

  if (command->out_file) {
    free(command->out_file);
    command->out_file = NULL;
  }

  if (command->in_file) {
    free(command->in_file);
    command->in_file = NULL;
  }

  if (command->err_file) {
    free(command->err_file);
    command->err_file = NULL;
  }

  command->append_out = false;
  command->append_err = false;
  command->background = false;

  free(command);
  command = NULL;
} /* free_command() */

/*
 *  Print the contents of the command in a pretty way.
 */

void print_command(command_t *command) {
  printf("\n\n");
  printf("              COMMAND TABLE                \n");
  printf("\n");
  printf("  #   single Commands\n");
  printf("  --- ----------------------------------------------------------\n");

  // iterate over the single commands and print them nicely

  for (int i = 0; i < command->num_single_commands; i++) {
    printf("  %-3d ", i);
    print_single_command(command->single_commands[i]);
  }

  printf("\n\n");
  printf("  Output       Input        Error        Background\n");
  printf("  ------------ ------------ ------------ ------------\n");
  printf("  %-12s %-12s %-12s %-12s\n",
            command->out_file?command->out_file:"default",
            command->in_file?command->in_file:"default",
            command->err_file?command->err_file:"default",
            command->background?"YES":"NO");
  printf("\n\n");
} /* print_command() */

/*
 *  Execute a command. Manages input/output/error files for every case,
 *  including piping and multiple redirection. Also handles background
 *  processes.
 */

void execute_command(command_t *command) {
  /* Don't do anything if there are no single commands */

  if (command->single_commands == NULL) {
    print_prompt();
    return;
  }

  /* Set the last command as g_whole_command, which was pieced together
   * in shell.l. Specifically used for ${!!}
   */

  char * last_command = NULL;
  if (!strcmp(g_whole_command, "${$}")) {
    /* The PID of the shell process */

    char * env_return_val = getenv("SHELL_PID");
    if (env_return_val != NULL) {
      last_command = strdup(env_return_val);
    }
  }
  else if (!strcmp(g_whole_command, "${?}")) {
    /* The return code of the last executed simple command (ignoring commands
     * sent to the background).
     */

    char * env_return_val = malloc(sizeof(char));
    sprintf(env_return_val, "%d", g_last_return_code);
  }
  else if (!strcmp(g_whole_command, "${!}")) {
    /* The PID of the last process run in the background. */

    char * env_return_val = getenv("LAST_BG_PID");
    if (env_return_val != NULL) {
      last_command = strdup(env_return_val);
    }
  }
  else if (!strcmp(g_whole_command, "${!!}")) {
    /* Run the last command again. */

    if (g_last_command != NULL) {
      last_command = strdup(g_last_command);
    }
  }
  else if (!strcmp(g_whole_command, "${_}")) {
    /* The last argument in the fully expanded previous command. Note that this
     * excludes redirects.
     */

    if (g_last_argument != NULL) {
      last_command = strdup(g_last_argument);
    }
  }
  else if (!strcmp(g_whole_command, "${SHELL}")) {
    /* The path of your shell executable. */

    last_command = strdup(g_shell_path);
  }
  else {
    /* Env var is not a special case. */

    last_command = strdup(g_whole_command);
  }

  if (last_command == NULL) {
    last_command = "";
  }

  g_last_command = strdup(last_command);

  insert_into_history(strdup(last_command));

  g_whole_command = strdup("");
  g_whole_command_size = 0;
  free(last_command);
  last_command = NULL;

  /* setenv(), unsetenv(), and cd() are outside of fork() because they do not
   * modify the state of the parent. If they were called inside fork(), the
   * changes would not be reflected in the parent.
   */

  if (!strcmp(command->single_commands[0]->arguments[0], "setenv")) {
    if ((command->single_commands[0]->num_args < 3) ||
        (setenv(command->single_commands[0]->arguments[1],
         command->single_commands[0]->arguments[2], 1))) {
      perror("setenv");
    }
    return;
  }
  else if (!strcmp(command->single_commands[0]->arguments[0], "unsetenv")) {
    if (unsetenv(command->single_commands[0]->arguments[1])) {
      perror("unsetenv");
    }
    return;
  }
  else if (!strcmp(command->single_commands[0]->arguments[0], "cd")) {
    if (command->single_commands[0]->num_args > 1) {
      /* There is an explicit path to cd to */

      if (chdir(command->single_commands[0]->arguments[1])) {
        fprintf(stderr, "cd: can't cd to %s\n",
            command->single_commands[0]->arguments[1]);
      }
    }
    else {
      /* No explicit path specified, just cd to $HOME */

      char ** environ_p = environ;
      while (*environ_p != NULL) {
        if (strncmp("HOME=", *environ_p, 5) == 0) {
          break;
        }
        environ_p++;
      }

      char * home = (char *) malloc(strlen(*environ_p) - 5);
      strcpy(home, *environ_p + 5);

      if (chdir(home)) {
        perror("chdir");
        g_last_return_code = 2;
      }
    }
    return;
  }

  int default_in = dup(0);
  if (default_in == -1) {
    perror("dup");
    exit(EXIT_FAILURE);
  }
  int default_out = dup(1);
  if (default_out == -1) {
    perror("dup");
    exit(EXIT_FAILURE);
  }
  int default_err = dup(2);
  if (default_err == -1) {
    perror("dup");
    exit(EXIT_FAILURE);
  }

  /* Declares the file descriptors that will be used for 'command'. */

  int input_fd = 0;
  int output_fd = 0;
  int err_fd = 0;

  /* SET STARTING INPUT FILE */

  if (command->in_file != NULL) {
    if (access(command->in_file, F_OK) != FILE_NOT_FOUND) {
      input_fd = open(command->in_file, O_RDONLY, 0444);
    }
    else {
      /* File does not exist. Throw an error. */

      perror(command->in_file);
      g_last_return_code = 1;
      return;
    }
  }
  else {
    input_fd = dup(default_in);
    if (input_fd == -1) {
      perror("dup");
      exit(EXIT_FAILURE);
    }
  }

  /* LOOP THROUGH SINGLE_COMMANDS */

  for (int i = 0; i < command->num_single_commands; i++) {
    single_command_t * current_single_command = command->single_commands[i];

    /* REDIRECT INPUT */

    if (dup2(input_fd, 0) == -1) {
      perror("dup2");
      exit(EXIT_FAILURE);
    }
    close(input_fd);

    /* SET UP OUTPUT */

    if (i == command->num_single_commands - 1) {
      /* Last single command argument (important for output file) */

      g_last_argument = strdup(current_single_command->
          arguments[current_single_command->num_args - 1]);

      if (command->out_file != NULL) {
        if (command->append_out) {
          output_fd = open(command->out_file,
              O_RDWR | O_CREAT | O_APPEND, 0666);
        }
        else {
          output_fd = open(command->out_file, O_RDWR | O_CREAT | O_TRUNC, 0666);
        }
      }
      else {
        output_fd = dup(default_out);
        if (output_fd == -1) {
          perror("dup");
          exit(EXIT_FAILURE);
        }
      }
    }
    else {
      /* Not last single command */

      int pipe_fds[2];
      if (pipe(pipe_fds) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
      }
      input_fd = pipe_fds[0];
      output_fd = pipe_fds[1];
    }

    /* REDIRECT OUTPUT */

    if (dup2(output_fd, 1) == -1) {
      perror("dup2");
      exit(EXIT_FAILURE);
    }
    close(output_fd);

    if (command->err_file != NULL) {
      if (command->append_err) {
        err_fd = open(command->err_file, O_RDWR | O_CREAT | O_APPEND, 0666);
      }
      else {
        err_fd = open(command->err_file, O_RDWR | O_CREAT | O_TRUNC, 0666);
      }
    }
    else {
      err_fd = dup(default_err);
      if (err_fd == -1) {
        perror("dup");
        exit(EXIT_FAILURE);
      }
    }
    if (dup2(err_fd, 2) == -1) {
      perror("dup2");
      exit(EXIT_FAILURE);
    }
    close(err_fd);

    /* Forks the current single command and executes it */

    int fork_return = fork();
    if (fork_return == 0) {
      /* if fork() is successful, 0 is returned in the child, so if 0 is
       * returned, that means we are in the child process and can execute.
       */

      if (!strcmp(current_single_command->arguments[0], "printenv")) {
        char ** environ_p = environ;
        while (*environ_p != NULL) {
          printf("%s\n", *environ_p++);
        }
        g_last_return_code = 0;
        exit(EXIT_SUCCESS);
      }

      execvp(current_single_command->arguments[0],
          current_single_command->arguments);

      /* This perror() is only executed if execvp fails. If execvp doesn't fail,
       * I think we are transported back to the parent process?
       */

      perror("execvp");
      g_last_return_code = 2;
      close(default_in);
      close(default_out);
      close(default_err);
      exit(EXIT_SUCCESS);
    }
    else if (fork_return < 0) {
      /* There was an error in fork */

      perror("fork");
      g_last_return_code = 1;
      close(default_in);
      close(default_out);
      close(default_err);
      exit(EXIT_FAILURE);
    }

    if (!command->background) {
      /* This is the parent process */
      /* fork_return is the pid of the child */
      /* Wait until the child exits if it's not a background process */

      waitpid(fork_return, &g_last_return_code, 0);
    }
    else {
      /* Command is a background process, so put it in the g_background_process
       * integer array. This is so that we can only display '[PID] exited.' when
       * a background process exits.
       */

      g_total_bg_commands++;
      g_background_process = (int *) realloc(g_background_process,
                                             g_total_bg_commands);
      g_background_process[g_total_bg_commands - 1] = fork_return;
      g_last_background_pid = fork_return;

      /* Set the environment variable for ${!}, the PID of the most recent
       * command run in the background.
       */

      char * last_bg_pid = malloc(sizeof(char) * MAX_PID_LEN);
      sprintf(last_bg_pid, "%d", fork_return);
      setenv("LAST_BG_PID", last_bg_pid, 1);
      free(last_bg_pid);
      last_bg_pid = NULL;
    }

    /* Set the environment variable for ${?} the return code of the last
     * executed single command, excluding background processes.
     */

    g_last_return_code = WEXITSTATUS(g_last_return_code);
    char * last_return_code = malloc(sizeof(char) + 1);
    sprintf(last_return_code, "%d", g_last_return_code);
    setenv("LAST_RETURN", strdup(last_return_code), 1);
    free(last_return_code);
    last_return_code = NULL;
  }

  /* Free/close stuff to prepare for next command, then print another prompt.
   * dup2() calls restore the default_in, default_out, and default_err fd's
   */

  dup2(default_in, 0);
  dup2(default_out, 1);
  dup2(default_err, 2);
  close(default_in);
  close(default_out);
  close(default_err);

} /* execute_command() */
