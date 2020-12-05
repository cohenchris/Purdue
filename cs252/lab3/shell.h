#ifndef SHELL_H
#define SHELL_H

#include "command.h"
#include "single_command.h"
#include "read_line.h"

void print_prompt();
void expand_wildcards(char * prefix, char * suffix);
void update_whole_command(char * text, int leng);

extern command_t * g_current_command;
extern single_command_t * g_current_single_command;
extern int * g_background_process;
extern int g_total_bg_commands;
extern int g_using_original_buffer;
extern int g_last_return_code;
extern int g_last_background_pid;
extern char * g_shell_path; //for ${SHELL}

extern char * g_whole_command;
extern int g_whole_command_size;
#endif
