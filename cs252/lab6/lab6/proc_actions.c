#include "proc_actions.h"

#include <stdio.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>
#include <sys/types.h>
#include <signal.h>
#include <stdlib.h>

#define MAX_FILENAME (50)
#define MAX_INT_LEN (10)

int g_num_maps = 0;
int g_num_fds = 0;

int stop_proc(pid_t pid) {
  if (kill(pid, SIGSTOP) != 0) {
    perror("stop process");
    return -1;
  }
  return 0;
} /* stop_proc() */

int continue_proc(pid_t pid) {
  if (kill(pid, SIGCONT) != 0) {
    perror("continue process");
    return -1;
  }
  return 0;
} /* continue_proc() */

int kill_proc(pid_t pid) {
  if (kill(pid, SIGKILL) != 0) {
    perror("kill process");
    return -1;
  }
  return 0;
} /* kill_proc() */

mmap_info * list_mm(pid_t pid) {
  g_num_maps = 0;
  char * mmap_file_name = strdup("/proc/");
  mmap_file_name = realloc(mmap_file_name, sizeof(char) * MAX_FILENAME);
  char * pid_str = malloc(sizeof(char) * MAX_INT_LEN);
  sprintf(pid_str, "%d", pid);
  strcat(mmap_file_name, pid_str);
  strcat(mmap_file_name, "/maps");


  FILE * mmap_fp = fopen(mmap_file_name, "r");
  if (mmap_fp == NULL) {
    free(pid_str);
    pid_str = NULL;
    free(mmap_file_name);
    mmap_file_name = NULL;
    perror("invalid pid");
    return NULL;
  }

  char * line = NULL;
  size_t len = 0;
  ssize_t read_len = 0;
  int field_num = 1;
  char * field = NULL;
  mmap_info * mem_map = malloc(sizeof(mmap_info));
  while ((read_len = getline(&line, &len, mmap_fp)) != -1) {
    mem_map = realloc(mem_map, (g_num_maps + 1) * sizeof(mmap_info));

    field_num = 1;
    field = strtok(line, " ");

    mem_map[g_num_maps].addr = strdup(field);
    field = strtok(NULL, " ");

    mem_map[g_num_maps].perms = strdup(field);
    field = strtok(NULL, " ");

    mem_map[g_num_maps].offset = strdup(field);
    field = strtok(NULL, " ");

    mem_map[g_num_maps].dev = strdup(field);
    field = strtok(NULL, " ");

    mem_map[g_num_maps].inode = strdup(field);
    field = strtok(NULL, " ");

    mem_map[g_num_maps].pathname = strndup(field, strlen(field) - 1);
    field = strtok(NULL, " ");

    g_num_maps++;
  }

  free(mmap_file_name);
  mmap_file_name = NULL;
  free(pid_str);
  pid_str = NULL;
  free(line);
  line = NULL;

  fclose(mmap_fp);
  mmap_fp = NULL;

  if (mem_map[0].addr == NULL) {
    printf("Memory map not available.");
  }
  return mem_map;
} /* list_mm() */

open_files * list_open_files(pid_t pid) {
  g_num_fds = 0;
  char * files_name = strdup("/proc/");
  files_name = realloc(files_name, sizeof(char) * MAX_FILENAME);
  char * pid_str = malloc(sizeof(char) * MAX_INT_LEN);
  sprintf(pid_str, "%d", pid);
  strcat(files_name, pid_str);
  strcat(files_name, "/fd");

  DIR * dir = opendir(files_name);
  if (dir == NULL) {
    free(files_name);
    files_name = NULL;
    free(pid_str);
    pid_str = NULL;
    perror("invalid pid");
    return NULL;
  }

  open_files * open_proc_files = NULL;

  struct dirent * dir_entry = { 0 };
  while ((dir_entry = readdir(dir)) != NULL) {
    if (!strcmp(dir_entry->d_name, ".") || !strcmp(dir_entry->d_name, "..")) {
      /* Ignore . and .. entries because they aren't valid fds */
      continue;
    }
    open_proc_files = realloc(open_proc_files, (g_num_fds + 1) *
                                               sizeof(open_files));
    open_proc_files[g_num_fds].fd = strdup(dir_entry->d_name);
    //open_proc_files[g_num_fds].type = NULL;
    char * fd_path = strdup(files_name);
    fd_path = realloc(fd_path, sizeof(char) * MAX_FILENAME);
    strcat(fd_path, "/");
    strcat(fd_path, dir_entry->d_name);
    char * symlink_read = malloc(sizeof(char) * MAX_FILENAME);
    ssize_t num_bytes_written = readlink(fd_path, symlink_read, MAX_FILENAME);
    symlink_read[num_bytes_written] = '\0';
    open_proc_files[g_num_fds].object = strdup(symlink_read);

    g_num_fds++;

    free(symlink_read);
    free(fd_path);
    fd_path = NULL;
  }

  free(pid_str);
  pid_str = NULL;
  free(files_name);
  files_name = NULL;

  closedir(dir);
  dir = NULL;

  return open_proc_files;
} /* list_open_files() */
