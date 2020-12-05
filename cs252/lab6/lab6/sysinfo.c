#include "sysinfo.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/vfs.h>

#define KB_TO_GB (1048576)
#define MAX_INT_SIZE (20)

system_info g_sys_info = { 0 };

void get_release_info() {
  /* OS RELEASE INFORMATION */

  FILE * fp = fopen("/etc/os-release", "r");
  if (fp == NULL) {
    perror("couldn't read /etc/os-release");
    exit(EXIT_FAILURE);
  }

  char * line = NULL;
  size_t len = 0;
  ssize_t read_len = 0;
  char * first_quote = NULL;
  char * second_quote = NULL;
  while ((read_len = getline(&line, &len, fp)) != -1) {
    if (strstr(line, "NAME=") != NULL) {
      first_quote = strchr(line, '"');
      second_quote = strchr(first_quote + 1, '"');
      g_sys_info.release_name = strndup(first_quote + 1,
                                        second_quote - first_quote - 1);
      if (g_sys_info.release_version != NULL) {
        /* if release version already found too, we need no other information */
        break;
      }
    }
    else if (strstr(line, "VERSION=") != NULL) {
      char * first_quote = strchr(line, '"');
      char * second_quote = strchr(first_quote + 1, '"');
      g_sys_info.release_version = strndup(first_quote + 1,
                                           second_quote - first_quote - 1);
      if (g_sys_info.release_name != NULL) {
        /* if release name already found too, we need no other information */
        break;
      }
    }
  }

  fclose(fp);
  fp = NULL;
  free(line);
  line = NULL;

  /* KERNEL INFORMATION */

  fp = fopen("/proc/version_signature", "r");
  if (fp == NULL) {
    perror("couldn't read /proc/version_signature");
    exit(EXIT_FAILURE);
  }

  if ((read_len = getline(&line, &len, fp)) != -1) {
    g_sys_info.kernel_version = strndup(strstr(line, " ") + 1,
        strlen(strstr(line, " ") + 1) - 1);
  }
  else {
    perror("couldn't read /proc/version_signature");
    exit(EXIT_FAILURE);
  }

  fclose(fp);
  fp = NULL;
  free(line);
  line = NULL;
} /* get_release_info() */

void get_hardware_info() {
  /* RAM information */
  FILE * fp = fopen("/proc/meminfo", "r");
  if (fp == NULL) {
    perror("couldn't read /proc/meminfo");
    exit(EXIT_FAILURE);
  }


  char * line = NULL;
  size_t len = 0;
  ssize_t read_len = 0;
  while ((read_len = getline(&line, &len, fp)) != -1) {
    if (strstr(line, "MemTotal") != NULL) {
      char * start_mem_string = strchr(line, ' ');
      char * end_mem_string = strchr(line + 1, ' ');
      char * mem_string = strndup(start_mem_string + 1,
                                  end_mem_string - start_mem_string - 1);
      long int total_mem_kb = strtol(mem_string, NULL, 10);
      g_sys_info.ram_size = malloc(sizeof(char) * MAX_INT_SIZE);
      sprintf(g_sys_info.ram_size, "%li", total_mem_kb);
      /*
      long int total_mem_gb = total_mem_kb / KB_TO_GB;
      sprintf(g_sys_info.ram_size, "%li", total_mem_gb);
      */

      free(mem_string);
      mem_string = NULL;
      break;
    }
  }

  fclose(fp);
  fp = NULL;
  free(line);
  line = NULL;

  fp = fopen("/proc/cpuinfo", "r");
  if (fp == NULL) {
    perror("error reading /proc/cpuinfo");
    exit(EXIT_FAILURE);
  }

  char * cpu_name = NULL;
  char * num_cores = NULL;
  while ((read_len = getline(&line, &len, fp)) != -1) {
    if (strstr(line, "model name") != NULL) {
      if (cpu_name == NULL) {
        cpu_name = strndup(strchr(line, ':') + 2, strlen(strchr(line, ':') + 2) - 1);
      }
    }
    else if (strstr(line, "cpu cores") != NULL) {
      if (num_cores == NULL) {
        num_cores = strdup(strchr(line, ':') + 2);
      }
    }
  }

  cpu_name = realloc(cpu_name, strlen(cpu_name) + strlen(num_cores) + 5);
  strcat(cpu_name, " x ");
  strcat(cpu_name, num_cores);
  g_sys_info.cpu_info = strdup(cpu_name);

  free(cpu_name);
  cpu_name = NULL;
  free(num_cores);
  num_cores = NULL;
  fclose(fp);
  fp = NULL;
  free(line);
  line = NULL;
} /* get_hardware_info() */

void get_disk_info() {
  FILE * fp = fopen("/proc/mounts", "r");
  if (fp == NULL) {
    perror("couldn't read /proc/mounts");
    exit(EXIT_FAILURE);
  }

  char * line = NULL;
  size_t len = 0;
  ssize_t read_len = 0;
  char * mount_name = NULL;
  ssize_t total_size = 0;
  struct statfs fs_stats = { 0 };
  char * first_space = NULL;
  char * second_space = NULL;
  while ((read_len = getline(&line, &len, fp)) != -1) {
    first_space = strchr(line, ' ');
    second_space = strchr(first_space + 1, ' ');
    mount_name = strndup(first_space + 1, second_space - first_space - 1);
    if (statfs(mount_name, &fs_stats) == -1) {
      free(mount_name);
      mount_name = NULL;
      continue;
      /*
      printf("%d\n", errno);
      perror("statfs");
      exit(EXIT_FAILURE);
      */
    }

    //total_size += fs_stats.f_bfree;
    total_size += fs_stats.f_bavail;
    // ^^ gave the right size of 162gb that ``df -h --total`` gave, so I think this is right.
    free(mount_name);
    mount_name = NULL;
  }

  total_size *= 4; // converts from blocks to kb
  g_sys_info.disk_space = malloc(sizeof(char) * MAX_INT_SIZE);
  sprintf(g_sys_info.disk_space, "%li", total_size);
  free(line);
  line = NULL;
  fclose(fp);
  fp = NULL;
} /* get_disk_info() */

system_info get_system_info() {
  get_release_info();
  get_hardware_info();
  get_disk_info();
  return g_sys_info;
} /* get_system_info() */
