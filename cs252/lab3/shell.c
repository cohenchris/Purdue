#include "shell.h"

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdbool.h>

#include "command.h"
#include "single_command.h"

#define BUFFER_SIZE (4096)
#define UNUSED(x) (void)(x)
#define MAX_PID_LEN (20)

command_t *g_current_command = NULL;
single_command_t *g_current_single_command = NULL;
int g_total_bg_commands = 0;
int * g_background_process = NULL;
int yyparse(void);
char * g_shell_path = NULL;
extern int g_using_original_buffer;
extern char g_line_buffer[];
extern int g_line_length;
extern int g_cursor_position;
extern bool g_using_r_search;

/*
 * Handles SIGINT signals (for when ctrl-C is pressed).
 */

extern void handle_sigint() {
  /* g_line_buffer is the buffer for read_line.c. If ctrl-C is pressed, clear
   * the buffer because we don't want to store the previously typed in string
   * into history.
   */

  memset(g_line_buffer, 0, MAX_BUFFER_LINE);
  g_line_length = 0;
  g_cursor_position = 0;
  g_using_r_search = false;

  printf("\n");
  print_prompt();
} /* handle_sigint() */

/*
 * Handles SIGCHILD signals (zombie processes).
 */

extern void handle_sigchld(int sig, siginfo_t * info, void * ucontext) {
  /* The first argument of waitpid() being -1 means it waits for any child
   * process, rather than a specific one. The NULL second argument means it
   * doesn't store any exit information about the process into any variable. The
   * WNOHANG makes it so that the shell doesn't hang while waiting for any
   * background process to exit.
   */

  while (waitpid(-1, NULL, WNOHANG) > EXIT_SUCCESS) {
    continue;
  }

  /* These useless statements shuts up the Makefile because it screams at me
   * for 'unused arguments'.
   */

  UNUSED(ucontext);
  UNUSED(sig);

  /* If the command is a background process, print the exit statement that
   * shows which process exited.
   */
  for (int i = 0; i < g_total_bg_commands; i++) {
    if (g_background_process[i] == info->si_pid) {
      printf("[%d] exited.\n", info->si_pid);
    }
  }
} /* handle_sigchld() */

/*
 *  Prints shell prompt, except in the case where the shell is passed arguments
 *  through the bash command line (rather than in the shell program itself).
 */

void print_prompt() {
  int input_fd = dup(0);
  if (isatty(input_fd) && !g_using_original_buffer) {
    printf("myshell> ");
    fflush(stdout);
  }
  close(input_fd);
} /* print_prompt() */

/*
 *  This main is simply an entry point for the program which sets up
 *  memory for the rest of the program and the turns control over to
 *  yyparse and never returns
 */

int main(int argc, char ** argv) {
  UNUSED(argc);
  g_current_command = (command_t *) malloc(sizeof(command_t));
  /* pointer error here? */
  g_current_single_command =
        (single_command_t *) malloc(sizeof(single_command_t));

  create_command(g_current_command);
  create_single_command(g_current_single_command);

  /* Signal Handler for SIGINT (ctrl-C) */

  struct sigaction sigint_struct = {0};
  sigint_struct.sa_handler = handle_sigint;
  sigemptyset(&sigint_struct.sa_mask);
  sigint_struct.sa_flags = SA_RESTART;

  if (sigaction(SIGINT, &sigint_struct, NULL)) {
    perror("sigint");
    g_last_return_code = 1;
    free(g_background_process);
    g_background_process = NULL;
    free_command(g_current_command);
    g_current_command = NULL;
    free_single_command(g_current_single_command);
    g_current_single_command = NULL;
    exit(EXIT_FAILURE);
  }

  /* Signal Handler for SIGCHLD (zombie processes) */

  struct sigaction zombie_struct = {0};
  zombie_struct.sa_flags = SA_SIGINFO | SA_RESTART;
  zombie_struct.sa_sigaction = handle_sigchld;
  sigemptyset(&zombie_struct.sa_mask);

  if (sigaction(SIGCHLD, &zombie_struct, NULL)) {
    perror("sigchild");
    g_last_return_code = 1;
    free(g_background_process);
    g_background_process = NULL;
    free_command(g_current_command);
    g_current_command = NULL;
    free_single_command(g_current_single_command);
    g_current_single_command = NULL;
    exit(EXIT_FAILURE);
  }

  /* source .shellrc (like bash does with .bashrc) */

  if (!access(".shellrc", F_OK)) {
    FILE * shellrc = fopen(".shellrc", "r");
    if (shellrc != NULL) {
      char * line = (char *) malloc(BUFFER_SIZE);
      while (fgets(line, BUFFER_SIZE, shellrc)) {
        if (system(line)) {
          perror(line);
          g_last_return_code = 1;
          exit(EXIT_FAILURE);
        }
      }
      fclose(shellrc);
      shellrc = NULL;
      free(line);
      line = NULL;
    }
  }

  /* Keeps track of the shell path for later use with ${SHELL} */

  g_shell_path = realpath(argv[0], NULL);
  char * shell_pid = malloc(sizeof(char) * MAX_PID_LEN);
  sprintf(shell_pid, "%d", getpid());
  setenv("SHELL_PID", strdup(shell_pid), 1);
  setenv("LAST_RETURN", "0", 1);

  free(shell_pid);
  shell_pid = NULL;

  print_prompt();
  yyparse();
} /* main() */
