#include <stdlib.h>
#include <stdio.h>

typedef struct {
  char * addr;
  char * perms;
  char * offset;
  char * dev;
  char * inode;
  char * pathname;
} mmap_info;

typedef struct {
  char * fd;
  //char * type;
  char * object;
} open_files;

int stop_proc(pid_t pid);
int continue_proc(pid_t pid);
int kill_proc(pid_t pid);
mmap_info * list_mm(pid_t pid);
open_files * list_open_files(pid_t pid);

int g_num_maps;
int g_num_fds;
