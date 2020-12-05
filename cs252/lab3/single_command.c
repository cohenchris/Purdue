#include "single_command.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern int g_last_return_code;
extern char * g_last_argument;
extern char * g_last_command;

/*
 *  Initialize a single command.
 */

void create_single_command(single_command_t *simp) {
  simp->arguments = NULL;
  simp->num_args = 0;
} /* create_single_command() */

/*
 *  Free everything in a single command.
 */

void free_single_command(single_command_t *simp) {
  for (int i = 0; i < simp->num_args; i++) {
    free(simp->arguments[i]);
    simp->arguments[i] = NULL;
  }

  free(simp->arguments);
  simp->arguments = NULL;

  simp->num_args = 0;

  free(simp);
  simp = NULL;
} /* free_single_command() */

/*
 *  Insert an argument into the list of arguments in a single command. Makes
 *  sure the argument list is always NULL-terminated.
 */

void insert_argument(single_command_t *simp, char *argument) {
  if (argument == NULL) {
    return;
  }

  /* EXPAND ENVIRONMENT VARIABLES */

  char * final_argument = NULL;
  if (strchr(argument, '$')) {
    for (int i = 0; i < (int) strlen(argument); i++) {
      if ((argument[i] == '$') &&
          (i < (int) strlen(argument) - 3) &&
          (argument[i + 1] == '{')) {
        char * next_curly_brace = strchr(argument + i, '}');

        char * env_var_str = (char *) malloc(sizeof(char) * (next_curly_brace -
                                                             argument - i - 1));

        /* env_var_str is the extracted string from inside ${ } */

        strncpy(env_var_str, argument + i + 2, next_curly_brace -
            argument - i - 2);
        env_var_str[next_curly_brace - argument - i - 2] = '\0';

        char * env_var = NULL;
        if (!strcmp(env_var_str, "$")) {
          char * env_return_val = getenv("SHELL_PID");
          if (env_return_val != NULL) {
            env_var = strdup(env_return_val);
          }
        }
        else if (!strcmp(env_var_str, "?")) {
          char * env_return_val = getenv("LAST_RETURN");
          if (env_return_val != NULL) {
            env_var = strdup(env_return_val);
          }
          /*
          char * env_return_val = malloc(sizeof(char));
          sprintf(env_return_val, "%d", g_last_return_code);
          */
        }
        else if (!strcmp(env_var_str, "!")) {
          char * env_return_val = getenv("LAST_BG_PID");
          if (env_return_val != NULL) {
            env_var = strdup(env_return_val);
          }
        }
        else if (!strcmp(env_var_str, "!!")) {
          if (g_last_command != NULL) {
            env_var = strdup(g_last_command);
          }
        }
        else if (!strcmp(env_var_str, "_")) {
          if (g_last_argument != NULL) {
            env_var = strdup(g_last_argument);
          }
        }
        else if (!strcmp(env_var_str, "SHELL")) {
          env_var = strdup(g_shell_path);
        }
        else {
          if (getenv(env_var_str) != NULL) {
            env_var = strdup(getenv(env_var_str));
          }
        }

        if (env_var == NULL) {
          /* No environment variable corresponding to the passed in variable, so
           * just substitute an empty string.
           */

          env_var = "";
        }

        if (final_argument != NULL) {
          final_argument = realloc(final_argument, strlen(final_argument) +
              strlen(env_var) + 1);
          final_argument = strcat(final_argument, env_var);
        }
        else {
          final_argument = strdup(env_var);
        }

        i += strlen(env_var_str) + 2;

        free(env_var_str);
        env_var_str = NULL;
      }
      else {
        if (final_argument == NULL) {
          final_argument = malloc(sizeof(char));
          final_argument[0] = argument[i];
        }
        else {
          final_argument = realloc(final_argument,
                                   sizeof(char) * (strlen(final_argument) + 1));
          final_argument[strlen(final_argument)] = argument[i];
        }
      }
    }
  }
  if (final_argument == NULL) {
    final_argument = strdup(argument);
  }

  /* END EXPAND ENVIRONMENT VARIABLES */

  simp->num_args++;

  /* Allocate (num_args + 1) space to make room for NULL terminator */

  simp->arguments = (char **) realloc(simp->arguments,
                                    (simp->num_args + 1) * sizeof(char *));
  simp->arguments[simp->num_args - 1] = strdup(final_argument);

  /* NULL terminate arguments list (needed for execvp()) */

  simp->arguments[simp->num_args] = NULL;

  free(final_argument);
  final_argument = NULL;
} /* insert_argument() */

/*
 *  Print a single command in a pretty format.
 */

void print_single_command(single_command_t *simp) {
  for (int i = 0; i < simp->num_args; i++) {
    printf("\"%s\" \t", simp->arguments[i]);
  }

  printf("\n\n");
} /* print_single_command() */
