#ifndef READ_LINE_H
#define READ_LINE_H

#define MAX_BUFFER_LINE (2048)

char *read_line();
void read_line_print_usage();


extern int g_line_length;
extern char g_line_buffer[];
int g_cursor_position;
extern int g_history_index;
extern char ** g_history;
extern int g_history_length;

#endif
