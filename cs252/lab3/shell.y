/*
 *
 * CS-252
 * shell.y: parser for shell
 *
 */

%union
{
  char * string;
};

/* Any string caught */
%token <string> WORD

/*  &  |  \n  */
%token BACKGROUND PIPE NEWLINE

/*  >  >>  <  */
%token REDIRECT APPEND PASS_IN

/*  2>  >&  >>&  */
%token REDIRECT_STD_ERR STDOUT_STDERR_TO_FILE STDOUT_STDERR_APPEND_FILE

%{

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>  
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <stdlib.h>
#include <regex.h>
#include <dirent.h>
#include <ctype.h>

#include "command.h"
#include "single_command.h"
#include "shell.h"

#define MAXFILENAME (1024)

void yyerror(const char * s);
int yylex();
int g_is_wildcard = 0;
char ** g_expanded_wildcard_args = NULL;
int g_num_wildcard_args = 0;

/*
 * Comparison function for use with qsort() after wildcard arguments have been
 * fully expanded.
 */

int str_cmp(const void * arg1, const void * arg2) {
  return strcmp(* (char * const *) arg1, * (char * const *) arg2);
}

%}

%%

goal:
  entire_command_list
  ;

entire_command_list:
     entire_command_list entire_command
  |  entire_command
  ;

entire_command:
  single_command_list io_modifier_list background NEWLINE {
    execute_command(g_current_command);
    free_command(g_current_command);
    g_current_command = (command_t *) malloc(sizeof(command_t));
    create_command(g_current_command);
    print_prompt();
  }
  | NEWLINE { print_prompt(); }
  ;

single_command_list:
  /* Makes sure commands with pipes are parsed correctly */
  single_command_list PIPE single_command
  | single_command
  ;

single_command:
  executable argument_list {
    insert_single_command(g_current_command, g_current_single_command);

    /* Reallocates single command so we can use it again */

    g_current_single_command =
      (single_command_t *) malloc(sizeof(single_command_t));
    create_single_command(g_current_single_command);
  }
  ;

executable:
  WORD {
    /* Sets the command to be executed in the single_command struct.
     * Worth noting that the executable will always be the first argument
     * in this struct.
     */

    insert_argument(g_current_single_command, strdup(yylval.string));
  }
;

argument_list:
  /* Recursively adds arguments to argument list in single_command struct */
  argument_list argument
  | /* can be empty */
  ;

argument:
  WORD {
    /* Adds an argument to the argument list in the single_command struct */

    char * new_arg = strdup(yylval.string);

    expand_wildcards(NULL, new_arg);

    if (g_is_wildcard) {
      qsort(g_expanded_wildcard_args, g_num_wildcard_args, sizeof(char *),
            str_cmp);
      g_is_wildcard = 0;
    }

    for (int i = 0; i < g_num_wildcard_args; i++) {
      insert_argument(g_current_single_command,
                      strdup(g_expanded_wildcard_args[i]));
    }

    /* If there are wildcards in the argument but it doesn't find anything,
     * put in the original argument including wildcard characters.
     */

    if (g_num_wildcard_args == 0) {
      /* If the only argument in g_current_single_command is the executable,
       * the wildcard expansion didn't find anything, so just put the string
       * with wildcards into the argument array.
       */

      insert_argument(g_current_single_command, strdup(yylval.string));
    }

    free(g_expanded_wildcard_args);
    g_expanded_wildcard_args = NULL;
    g_num_wildcard_args = 0;
    free(new_arg);
    new_arg = NULL;
  }
  ;

background:
  /* Sets previous command as a background process */
  BACKGROUND {
    g_current_command->background = 1;
  }
  | /* can be empty */
  ;

io_modifier_list:
  /* Recursively adds I/O modifiers to update command struct */
  io_modifier_list io_modifier
  | /* can be empty */
  ;

io_modifier:
  /* Takes following I/O modifiers into account:  >  >>  <  2>  >&  >>&  */

  /* > */
  REDIRECT WORD                    {
    if (g_current_command->out_file != NULL) {
      creat(g_current_command->out_file, 0666);
      printf("Ambiguous output redirect.\n");
    }
    g_current_command->out_file = strdup(yylval.string);
  }
  /* >> */
  | APPEND WORD                    {
    if (g_current_command->out_file != NULL) {
      creat(g_current_command->out_file, 0666);
      printf("Ambiguous output redirect.\n");
    }
    g_current_command->out_file = strdup(yylval.string);
    g_current_command->append_out = 1;
  }
  /* < */
  | PASS_IN WORD                   {
    g_current_command->in_file = strdup(yylval.string);
  }
  /* 2> */
  | REDIRECT_STD_ERR WORD          {
    g_current_command->err_file = strdup(yylval.string);
  }
  /* >& */
  | STDOUT_STDERR_TO_FILE WORD     {
    if (g_current_command->out_file != NULL) {
      creat(g_current_command->out_file, 0666);
      printf("Ambiguous output redirect.\n");
    }
    g_current_command->out_file = strdup(yylval.string);
    g_current_command->err_file = strdup(yylval.string);
  }
  /* >>& */
  | STDOUT_STDERR_APPEND_FILE WORD {
    if (g_current_command->out_file != NULL) {
      creat(g_current_command->out_file, 0666);
      printf("Ambiguous output redirect.\n");
    }
    g_current_command->out_file = strdup(yylval.string);
    g_current_command->err_file = strdup(yylval.string);
    g_current_command->append_out = 1;
    g_current_command->append_err = 1;
  }
  ;

%%

void
yyerror(const char * s)
{
  fprintf(stderr,"%s", s);
}

/*
 * Takes in each argument and expands it if there are wildcards involved. If
 * not, the argument is inserted normally without alteration.
 */

void expand_wildcards(char * prefix, char * suffix) {
  /* If suffix is empty, wildcards are done being parsed. */
  
  if (suffix[0] == 0) {
    g_num_wildcard_args++;
    g_expanded_wildcard_args = realloc(g_expanded_wildcard_args, sizeof(char *) *
                                       g_num_wildcard_args);
    g_expanded_wildcard_args[g_num_wildcard_args - 1] = strdup(prefix);
    return;
  }

  char * next_slash_index = strchr(suffix, '/');

  char * component = (char *) malloc(sizeof(char) * MAXFILENAME);
  if (next_slash_index != NULL) {
    /* If there is another slash, we still have more of the string to parse, so
     * capture up until the next slash. Update the suffix pointer to where the
     * next slash is.
     */

    strncpy(component, suffix, next_slash_index - suffix);
    component[next_slash_index - suffix] = '\0';
    suffix = next_slash_index + 1;
  }
  else {
    /* If there isn't another slash, this is the last part of the argument, so
     * copy the rest of it. Update the suffix pointer to NULL since there is
     * nothing left to parse after this.
     */

    strcpy(component, suffix);
    suffix = suffix + strlen(suffix);
  }

  char * new_prefix = (char *) malloc(sizeof(char) * MAXFILENAME);
  if (strchr(component, '*') == NULL && strchr(component, '?') == NULL) {
    if (prefix == NULL) {
      sprintf(new_prefix, "%s", component);
    }
    else {
      sprintf(new_prefix, "%s/%s", prefix, component);
    }
    expand_wildcards(new_prefix, suffix);
    free(component);
    component = NULL;
    free(new_prefix);
    new_prefix = NULL;
    return;
  }
  else {
    g_is_wildcard = 1;
  }

  /* Wildcard to Regex converter - given to us in Gustavo's notes */

  char * reg = (char *) malloc(2 * strlen(component) + 10);
  char * a = component;
  char * r = reg;
  *r = '^';
  r++;

  /* Substitution for wildcard format to regex */

  while (*a) {
    if (*a == '*') {
      *r++ = '.';
      *r++ = '*';
    }
    else if (*a == '?') {
      *r++ ='.';
    }
    else if (*a == '.') {
      *r++ = '\\';
      *r++ = '.';
    }
    else {
      *r++ = *a;
    }
    a++;
  }
  *r++ = '$';
  *r = '\0';

  /* END OF SUBSTITUTION */ 

  regex_t re;
  int status = regcomp(&re, reg, REG_EXTENDED | REG_NOSUB);
  if (status != 0) {
    free(component);
    component = NULL;
    free(new_prefix);
    new_prefix = NULL;
    free(reg);
    reg = NULL;
    return;
  }

  char * current_dir;

  if (prefix == NULL) {
    current_dir = ".";
  }
  else if (!strcmp(prefix, "")){
    current_dir = "/";
  }
  else {
    current_dir = strdup(prefix);
  }

  DIR * dir = opendir(strdup(current_dir));
  if (dir == NULL) {
    free(component);
    component = NULL;
    free(new_prefix);
    new_prefix = NULL;
    free(reg);
    reg = NULL;
    return;
  }

  struct dirent * ent;

  while ((ent = readdir(dir)) != NULL) {
    if (regexec(&re, ent->d_name, (size_t) 0, NULL, 0) == 0) {
      if (prefix == NULL) {
        sprintf(new_prefix, "%s", ent->d_name);
      }
      else {
        sprintf(new_prefix, "%s/%s", prefix, ent->d_name);
      }

      if (ent->d_name[0] == '.') {
        /* Handles the special case of dotfiles since we only want them to show
         * up when the begining of the component is also a period. */

        if (component[0] == '.') {
          expand_wildcards(new_prefix, suffix);
        }
      }
      else {
        expand_wildcards(new_prefix, suffix);
      }
    }
  }

  closedir(dir);
  dir = NULL;
  free(component);
  component = NULL;
  free(new_prefix);
  new_prefix = NULL;
  free(reg);
  reg = NULL;
} /* expand_wildcards() */

#if 0
main()
{
  yyparse();
}
#endif
