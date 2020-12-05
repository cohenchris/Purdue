/*
 *
 * CS-252 
 * shell.l: lexical analyzer for shell
 *
 */

%{

#include "shell.h"

#include "y.tab.h"
#include "command.h"
#include "single_command.h"

#include <sys/types.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
#include <pwd.h>

#define MAX_PID_LEN (20)
#define MAX_COMMAND_LEN (100)

extern char ** environ;
int g_using_original_buffer = 0;
extern char * g_shell_path;
extern int g_last_background_pid;
extern int g_last_return_code;
extern char ** g_history;
extern int g_history_length;
extern char * read_line();
extern char * g_last_command;
char * g_whole_command;
int g_whole_command_size = 0;

int mygetc(FILE * f) {
  static char * p;
  char ch;

  if (!isatty(0)) {
    //stdin is not a tty. Call real getc

    return getc(f);
  }

  //stdin is a tty. Call out read_line

  if (p == NULL || *p == 0) {
    char * s = read_line();
    p = s;
  }

  ch = *p;
  p++;

  return ch;
}

#undef getc
#define getc(f) mygetc(f)

static  void yyunput (int c, char * buf_ptr);

void myunputc(int c) {
  unput(c);
}

/*
 * Keeps track of the entire command being parsed by lex.
 */

void update_whole_command(char * text, int leng) {
  g_whole_command_size += leng;
  g_whole_command = (char *) realloc(g_whole_command,
                                     sizeof(char) * (g_whole_command_size + 1));

  if (g_whole_command == NULL) {
    g_whole_command = strdup(text);
  }
  else {
    strcat(g_whole_command, strdup(text));
  }
} /* update_whole_command() */

%}

%option noyywrap

%%

"exit" {
  exit(EXIT_FAILURE);
}

 /* Run the last command again. */
\$\{!!\}    {
  for (int i = strlen(g_last_command); i >= 0; i--) {
    myunputc(g_last_command[i]);
  }
}

 /* SUBSHELLS AAAAAAAAAAAAAAAAAAA */
\$\(.+\) {
  update_whole_command(yytext, yyleng);

  char * subshell_command = strdup(yytext + 2);
  subshell_command[yyleng - 3] = '\0';

  int default_in = dup(0);
  int default_out = dup(1);
  
  /* Pipes for parent (shell) and child (subshell) processes to communicate.
   * index 0 for pin and pout is for reading, index 1 is for writing.
   * pin[1] is the shell writing the command to the subshell, read by pin[0].
   * pout[1] is the subshell writing the output to the shell, read by pout[0].
   */

  int pin[2];
  int pout[2];
  int pin_fd = pipe(pin);
  int pout_fd = pipe(pout);
  if (pin_fd == -1 || pout_fd == -1) {
    perror("pipe");
    g_last_return_code = 1;
    exit(EXIT_FAILURE);
  }

  write(pin[1], subshell_command, strlen(subshell_command));
  write(pin[1], "\nexit\n", 6);
  close(pin[1]);

  /* Set input/output streams to the subshell's streams */

  dup2(pin[0], 0);
  close(pin[0]);
  dup2(pout[1], 1);
  close(pout[1]);

  int fork_return = fork();
  char ** shell_args = (char **) malloc(sizeof(char *) * 2);
  if (fork_return == 0) {
    shell_args[0] = subshell_command;
    shell_args[1] = NULL;
    execvp(g_shell_path, shell_args);
    perror("execvp");
    g_last_return_code = 1;
    exit(EXIT_FAILURE);
  }
  else if (fork_return < 0) {
    perror("fork");
    g_last_return_code = 1;
    close(default_in);
    close(default_out);
    exit(EXIT_FAILURE);
  }

  waitpid(fork_return, &g_last_return_code, 0);

  /* Reset input/output streams to the original ones just in case */

  dup2(default_in, 0);
  dup2(default_out, 1);
  close(default_in);
  close(default_out);

  char subshell_response_char = '\0';
  char * subshell_buffer = malloc(sizeof(char));
  int subshell_buffer_index = 0;

  while (read(pout[0], &subshell_response_char, sizeof(char)) != 0) {
    subshell_buffer = realloc(subshell_buffer, strlen(subshell_buffer) +
                                               sizeof(char) + 1);
    if (subshell_response_char == '\n') {
      /* Converts output into input for the command. If there are newlines, that
       * will cause the shell to think somebody wants to execute the command
       * that's been typed so far, and we want it to wait until the end.
       */

      subshell_buffer[subshell_buffer_index] = ' ';
    }
    else {
      subshell_buffer[subshell_buffer_index] = subshell_response_char;
    }
    subshell_buffer_index++;
  }
  subshell_buffer[subshell_buffer_index - 1] = '\0';

  for (int i = strlen(subshell_buffer); i >= 0; i--) {
    /* Puts each character back into the input stream. */

    myunputc(subshell_buffer[i]);
  }

  close(pout[0]);
  close(default_in);
  close(default_out);
  free(subshell_command);
  subshell_command = NULL;
  free(subshell_buffer);
  subshell_buffer = NULL;

  /* hardcoded shell_args freeing because it's always 2 arguments */

  free(shell_args[0]);
  shell_args[0] = NULL;
  free(shell_args[1]);
  shell_args[1] = NULL;
}

 /* SOURCE COMMAND */
source([ \t\n]+.*)? {
  update_whole_command(yytext, yyleng);

  if (access(yytext + 7, F_OK)) {
    perror("fopen");
    g_last_return_code = 1;
  }
  else {
    yyin = fopen(yytext + 7, "r");
    if (!yyin) {
      perror("fopen");
      g_last_return_code = 1;
    }
    /* Pushing the created buffer preserves the current buffer for later use */

    yypush_buffer_state(yy_create_buffer(yyin, YY_BUF_SIZE));
    g_using_original_buffer = 1;
    BEGIN(INITIAL);
  }
}

<<EOF>> {
  yypop_buffer_state();
  g_using_original_buffer = 0;
  if (!YY_CURRENT_BUFFER) {
    yyterminate();
  }
}

[^ \t\n]*\\[\#\$\!\(\)&<>\|\"\'\;\\\ \t\n][^ \t\n]* {
  update_whole_command(yytext, yyleng);

  char * escape_char_str = strdup(yytext);
  char * new_escape_str = malloc(yyleng * sizeof(char));
  int new_str_index = 0;
  int new_str_len = 0;
  for (int i = 0; i < yyleng; i++) {
    if (escape_char_str[i] == '\\') {
      new_escape_str[new_str_index] = escape_char_str[i + 1];
      i++;
      new_str_index++;
      new_str_len++;
    }
    else {
      new_escape_str[new_str_index] = escape_char_str[i];
      new_str_index++;
      new_str_len++;
    }
  }
  new_escape_str = realloc(new_escape_str, new_str_len * sizeof(char));
  yylval.string = strdup(new_escape_str);
  free(new_escape_str);
  new_escape_str = NULL;
  free(escape_char_str);
  escape_char_str = NULL;
  return WORD;
}

\n    { return NEWLINE; }

[ \t] {
  /* Discard spaces and tabs */

  update_whole_command(yytext, yyleng);
}


 /* I/O MODIFIERS */
">"   {
  update_whole_command(yytext, yyleng);
  return REDIRECT;
}
">>"  {
  update_whole_command(yytext, yyleng);
  return APPEND;
}
"<"   {
  update_whole_command(yytext, yyleng);
  return PASS_IN;
}
"2>"  {
  update_whole_command(yytext, yyleng);
  return REDIRECT_STD_ERR;
}
">&"  {
  update_whole_command(yytext, yyleng);
  return STDOUT_STDERR_TO_FILE;
}
">>&" {
  update_whole_command(yytext, yyleng);
  return STDOUT_STDERR_APPEND_FILE;
}

"|"   {
  update_whole_command(yytext, yyleng);
  return PIPE;
}
"&"   {
  update_whole_command(yytext, yyleng);
  return BACKGROUND;
}


\"[^\"]*\" {
  /* Adds support for arguments enclosed in quotes. Everything caught between
   * the quotes acts as one argument.
   */

  update_whole_command(yytext, yyleng);

  yylval.string = strndup(yytext + 1, yyleng - 2);
  return WORD;
}

~(\/.*)? {
  /* Expand tilde case 1 - current user's home */

  update_whole_command(yytext, yyleng);

  char * caught_string = strdup(yytext + 1);
  char * home = strdup(getenv("HOME"));
  if (!home) {
    perror("getenv");
    g_last_return_code = 1;
  }

  /* The second argument in realloc is the new size. (strlen(*environ_p) - 5)
   * corresponds to the old size of 'home'. (yyleng - 1) corresponds to the size
   * of the string to be appended (direct path).
   */

  home = realloc(home, sizeof(char) * (strlen(home) + strlen(caught_string)));
  home = strcat(home, caught_string);
  yylval.string = strdup(home);
  return WORD;
}

~[^ \t\n]* {
  /* Expand tilde case 2 - not current user's home */

  update_whole_command(yytext, yyleng);

  char * caught_string = strdup(yytext + 1);
  char * first_slash = strchr(yytext + 1, '/');
  if (first_slash == NULL) {
    first_slash = "";
  }
  char * user_string = strndup(yytext + 1, strlen(caught_string) -
      strlen(first_slash));
  char * suffix = strdup(first_slash);

  struct passwd * user_info = getpwnam(user_string);
  if (user_info == NULL) {
    perror("getpwnam");
    g_last_return_code = 2;
    return(EXIT_FAILURE);
  }

  yylval.string = strdup(user_info->pw_dir);
  yylval.string = strcat(yylval.string, suffix);

  free(caught_string);
  caught_string = NULL;
  free(user_string);
  user_string = NULL;
  free(suffix);
  suffix = NULL;
  return WORD;
}

([&]?[^ \t\n><\|\\&]|^(>>)|^(2>)|^(>&)|^(>>&))+ {
  /* Assume that file names have only alpha chars */
  /* Also makes sure input without spaces between file i/o modifiers are
   * accepted. */

  update_whole_command(yytext, yyleng);

  yylval.string = strdup(yytext);
  return WORD;
}
