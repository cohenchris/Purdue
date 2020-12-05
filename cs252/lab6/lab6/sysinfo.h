#include <stdio.h>
#include <stdlib.h>

typedef struct {
  /* release information */
  char * release_name;
  char * release_version;
  char * kernel_version;

  /* hardware information */
  char * ram_size;
  char * cpu_info;

  /* disk space */
  char * disk_space;
} system_info;

void get_release_info();
void get_hardware_info();
void get_disk_info();
system_info get_system_info();
