#include <stdio.h>
#include <stdlib.h>

typedef struct {
  char * proc_name;
  size_t proc_id;
  size_t parent_id;
  char * proc_user;
  char * state;
  char * mem;
  char * virtual_mem;
  char * resident_mem;
  char * shared_mem;
  char * cpu_time;
  char * start_date;
} process_info;

process_info * get_proc_info();
void read_status_file(process_info * new_proc, char * proc_dir_path);
void get_time_info(process_info * new_proc, char * proc_dir_path);
int compare_procs(const void * p1, const void * p2);

int g_num_procs;
