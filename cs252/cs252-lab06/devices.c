#include "devices.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/vfs.h>

#define MAX_INT_LEN (10)

int g_num_devices = 0;
device_info * g_device_info;

device_info * get_device_info() {
  /* Clears the original processes when this function is called */
  g_num_devices = 0;
  if (g_device_info != NULL) {
    free(g_device_info);
    g_device_info = NULL;
  }

  FILE * mounts_file = fopen("/proc/mounts", "r");
  if (mounts_file == NULL) {
    perror("fopen");
    exit(EXIT_FAILURE);
  }

  char * line = NULL;
  size_t len = 0;
  ssize_t read_len = 0;
  char * field = NULL;

  while ((read_len = getline(&line, &len, mounts_file)) != -1) {
    device_info new_device = { 0 };
    // device name
    field = strtok(line, " ");
    new_device.device = strdup(field);
    // mount point
    field = strtok(NULL, " ");
    new_device.mount_point = strdup(field);

    // skip a field to get to the 4th entry, which contains size
    field = strtok(NULL, " ");
    field = strtok(NULL, " ");

    char * size_field = strstr(field, "size=");
    if (size_field == NULL) {
      new_device.num_blocks = strdup("0");
    }
    else {
      char * next_field_delim = strstr(strstr(size_field, "=") + 1, ",");
      if (next_field_delim == NULL) {
        next_field_delim = strstr(strstr(size_field, "=") + 1, " ");
      }
      new_device.num_blocks = strndup(strstr(size_field, "=") + 1,
          next_field_delim - strstr(size_field, "=") - 2);
    }

    /* DEVICE STORAGE INFORMATION */

    struct statfs fs_stats = { 0 };

    if (statfs(new_device.device, &fs_stats) == -1) {
      new_device.used = strdup("0");
      new_device.available = strdup(new_device.num_blocks);
      new_device.use_percent = strdup("0%");
    }
    else {
      //TODO: check if these are right?
      ssize_t total_space = fs_stats.f_blocks;
      ssize_t amt_avail = fs_stats.f_bavail;
      ssize_t amt_used = total_space - amt_avail;
      float percent_used = ((float) amt_used/(float) total_space) * 100.0;

      /* 1k-blocks */
      if (new_device.num_blocks == 0) {
        free(new_device.num_blocks);
        new_device.num_blocks = NULL;

        new_device.num_blocks = malloc(sizeof(char) * MAX_INT_LEN);
        sprintf(new_device.num_blocks, "%lu", total_space);
      }

      /* Used memory */
      new_device.used = malloc(sizeof(char) * MAX_INT_LEN);
      sprintf(new_device.used, "%lu", amt_used);

      /* Available memory */
      new_device.available = malloc(sizeof(char) * MAX_INT_LEN);
      sprintf(new_device.available, "%lu", amt_avail);

      /* Percent of memory used */
      new_device.use_percent = malloc(sizeof(char) * MAX_INT_LEN);
      sprintf(new_device.use_percent, "%.2lf", percent_used);
      strcat(new_device.use_percent, "%");
    }

    // add new device to list
    g_device_info = realloc(g_device_info,
                            sizeof(device_info) * (g_num_devices + 1));
    g_device_info[g_num_devices] = new_device;
    g_num_devices++;
  }

  fclose(mounts_file);
  mounts_file = NULL;
  free(line);
  line = NULL;

  return g_device_info;
} /* get_device_info() */
