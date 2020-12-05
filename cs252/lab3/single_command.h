#ifndef SINGLE_COMMAND_H
#define SINGLE_COMMAND_H

typedef struct single_command {
  char **arguments;
  int num_args;
} single_command_t;

void create_single_command(single_command_t *);
void free_single_command(single_command_t *);
void insert_argument(single_command_t *, char *);
void print_single_command(single_command_t *);

extern char * g_shell_path;

#endif
