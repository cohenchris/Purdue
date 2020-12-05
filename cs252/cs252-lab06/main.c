#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "sysinfo.h"
#include "procinfo.h"
#include "devices.h"
#include "proc_actions.h"
#include "graphinfo.h"

#define TEST_PROC (139732)

extern int g_num_procs;
extern int g_num_devices;

int main() {
  printf("#################################################################\n");
  printf("#                        SYSTEM INFO                            #\n");
  printf("#################################################################\n");
  system_info curr_sys_info = get_system_info();

  printf("---------- RELEASE ----------\n");
  printf("%s\n", curr_sys_info.release_name);
  printf("Release %s\n", curr_sys_info.release_version);
  printf("Kernel %s\n", curr_sys_info.kernel_version);

  printf("\n---------- HARDWARE ----------\n");
  printf("Memory: %s kB\n", curr_sys_info.ram_size);
  printf("Processor: %s\b", curr_sys_info.cpu_info);

  printf("\n---------- DISK ----------\n");
  printf("Available Disk Space: %s kB\n", curr_sys_info.disk_space);

  /* FREE SYSTEM INFO */

  free(curr_sys_info.release_name);
  curr_sys_info.release_name = NULL;
  free(curr_sys_info.release_version);
  curr_sys_info.release_version = NULL;
  free(curr_sys_info.kernel_version);
  curr_sys_info.kernel_version = NULL;
  free(curr_sys_info.ram_size);
  curr_sys_info.ram_size = NULL;
  free(curr_sys_info.cpu_info);
  curr_sys_info.cpu_info = NULL;
  free(curr_sys_info.disk_space);
  curr_sys_info.disk_space = NULL;

  printf("\n\n");
  printf("#################################################################\n");
  printf("#                        PROCESS INFO                           #\n");
  printf("#################################################################\n");
  printf("\n");

  process_info * curr_proc_info = get_proc_info();
  //while (curr_proc_info[num_procs].proc_name != NULL) {
  printf("%-10s%-10s%-15s%-15s%-15s%-15s%-15s%-15s%-15s%-15s%-15s\n",
         "proc_id", "parent id", "proc_user", "proc_name", "state", "mem",
         "virtual_mem", "resident_mem", "shared_mem", "cpu_time", "start_date");
  printf("%s\n",
      "---------------------------------------------------------------------------------------------------------------------------------------------");
  for (int i = 0; i < g_num_procs; i++) {
    printf("%-10zu%-10zu%-15s%-15s%-18s%-15s%-15s%-15s%-15s%-15s%-15s\n",
           curr_proc_info[i].proc_id,
           curr_proc_info[i].parent_id,
           curr_proc_info[i].proc_user,
           curr_proc_info[i].proc_name,
           curr_proc_info[i].state,
           curr_proc_info[i].mem,
           curr_proc_info[i].virtual_mem,
           curr_proc_info[i].resident_mem,
           curr_proc_info[i].shared_mem,
           curr_proc_info[i].cpu_time,
           curr_proc_info[i].start_date
           );
    i++;
  }

  /* FREE PROCESS INFO */

  for (int i = 0; i < g_num_procs; i++) {
    free(curr_proc_info[i].proc_name);
    curr_proc_info[i].proc_name = NULL;
    free(curr_proc_info[i].proc_user);
    curr_proc_info[i].proc_user = NULL;
    free(curr_proc_info[i].state);
    curr_proc_info[i].state = NULL;
    free(curr_proc_info[i].mem);
    curr_proc_info[i].mem = NULL;
    free(curr_proc_info[i].virtual_mem);
    curr_proc_info[i].virtual_mem = NULL;
    free(curr_proc_info[i].resident_mem);
    curr_proc_info[i].resident_mem = NULL;
    free(curr_proc_info[i].shared_mem);
    curr_proc_info[i].shared_mem = NULL;
    free(curr_proc_info[i].cpu_time);
    curr_proc_info[i].cpu_time = NULL;
    free(curr_proc_info[i].start_date);
    curr_proc_info[i].start_date = NULL;
  }
  free(curr_proc_info);
  curr_proc_info = NULL;


  printf("\n\n");
  printf("#################################################################\n");
  printf("#                         DEVICE INFO                           #\n");
  printf("#################################################################\n");
  printf("\n");
  device_info * curr_device_info = get_device_info();

  printf("%-35s%-10s%-10s%-10s%-7s%s\n",
         "device name", "1k blocks", "used", "available",
         "use %", "mount point");
  printf("%s\n",
      "------------------------------------------------------------------------------------------------------");

  for (int i = 0; i < g_num_devices; i++) {
    printf("%-35s%-10s%-10s%-10s%-7s%-20s\n",
           curr_device_info[i].device,
           curr_device_info[i].num_blocks,
           curr_device_info[i].used,
           curr_device_info[i].available,
           curr_device_info[i].use_percent,
           curr_device_info[i].mount_point);
  }

  /* FREE DEVICE INFO */

  for (int i = 0; i < g_num_devices; i++) {
    free(curr_device_info[i].device);
    curr_device_info[i].device = NULL;
    free(curr_device_info[i].mount_point);
    curr_device_info[i].mount_point = NULL;
    free(curr_device_info[i].num_blocks);
    curr_device_info[i].num_blocks = NULL;
    free(curr_device_info[i].used);
    curr_device_info[i].used = NULL;
    free(curr_device_info[i].available);
    curr_device_info[i].available = NULL;;
    free(curr_device_info[i].use_percent);
    curr_device_info[i].use_percent = NULL;
  }
  free(curr_device_info);
  curr_device_info = NULL;


  printf("\n\n");
  printf("#################################################################\n");
  printf("#                         MEMORY MAPS                           #\n");
  printf("#################################################################\n");
  printf("\n");

  printf("%-35s%-7s%-15s%-7s%-15s%s\n",
         "address", "perms", "offset", "dev", "inode", "pathname");
  printf("-------------------------------------------------------------------------------------------------------\n");
  mmap_info * mmap = list_mm(TEST_PROC);
  for (int i = 0; i < g_num_maps; i++) {
  printf("%-35s%-7s%-15s%-7s%-15s%s\n",
         mmap[i].addr,
         mmap[i].perms,
         mmap[i].offset,
         mmap[i].dev,
         mmap[i].inode,
         mmap[i].pathname);
    
    /* FREE MEM MAP INFO */

    free(mmap[i].addr);
    mmap[i].addr = NULL;
    free(mmap[i].perms);
    mmap[i].perms = NULL;
    free(mmap[i].offset);
    mmap[i].offset = NULL;
    free(mmap[i].dev);
    mmap[i].dev = NULL;
    free(mmap[i].inode);
    mmap[i].inode = NULL;
    free(mmap[i].pathname);
    mmap[i].pathname = NULL;
  }
  free(mmap);
  mmap = NULL;

  printf("\n\n");
  printf("#################################################################\n");
  printf("#                         OPEN FILES                            #\n");
  printf("#################################################################\n");
  printf("\n");

  open_files * open_fds = list_open_files(TEST_PROC);

  //printf("%-5s%-10s%s\n", "fd", "type", "object");
  printf("%-5s%s\n", "fd", "object");
  printf("---------------------\n");
  for (int i = 0; i < g_num_fds; i++) {
    //printf("%-5s%-10s%s\n",
    printf("%-5s%s\n",
           open_fds[i].fd,
           //open_fds[i].type,
           open_fds[i].object);

  }

  /* FREE OPEN FILE INFO */

  for (int i = 0; i < g_num_fds; i++) {
    free(open_fds[i].fd);
    open_fds[i].fd = NULL;
    //free(open_fds[i].type);
    //open_fds[i].type = NULL;
    free(open_fds[i].object);
    open_fds[i].object = NULL;
  }
  free(open_fds);
  open_fds = NULL;

  printf("\n\n");
  printf("#################################################################\n");
  printf("#                         GRAPH INFO                            #\n");
  printf("#################################################################\n");
  printf("\n");

  graph_info curr_graph_info = get_graph_info();

  printf("cpu usage: %Lf%% of all CPUs\n", curr_graph_info.cpu_usage);
  printf("mem usage: %.2Lf kB\n", curr_graph_info.mem_usage);
  printf("swap usage: %.2Lf kB\n", curr_graph_info.swap_usage);
  printf("network usage sent: %.2Lf\nnetwork usage recieved: %.2Lf\n",
         curr_graph_info.network_sent, curr_graph_info.network_recieved);
  printf("\n");

  return EXIT_SUCCESS;
} /* main() */
