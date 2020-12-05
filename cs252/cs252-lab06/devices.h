#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct {
  char * device;        // field 1 in 'df'

  char * num_blocks;
  char * used;
  char * available;
  char * use_percent;

  char * mount_point;   // field 6 in 'df'
} device_info;

device_info * get_device_info();

int g_num_devices;
