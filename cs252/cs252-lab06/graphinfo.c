#include "graphinfo.h"

#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

graph_info g_graph_info = {0};

void get_cpu() {
  // get average cpu usage, of all cpus
  g_graph_info.cpu_usage = 0;
  g_graph_info.cpu_round = 0;
  FILE *fp = NULL;
  long double user = 0;
  long double nice = 0;
  long double system = 0;
  long double idle = 0;
  long double iowait = 0;
  long double irq = 0;
  long double softirq = 0;
  long double first_total = 0;
  long double first_work = 0;
  long double second_total = 0;
  long double second_work = 0;
  long double whole_total = 0;
  long double whole_work = 0;
  long double cpu_percent = 0;
  int rounded_percent = 0;

  fp = fopen("/proc/stat", "r");
  if (fp == NULL) {
    printf("error opening file in graphinfo\n");
    exit(0);
  }
  fscanf(fp, "%*s %Lf %Lf %Lf %Lf %Lf %Lf %Lf\n", &user, &nice, &system, &idle,
    &iowait, &irq, &softirq);
  first_total = (user + nice + system + idle + iowait + irq + softirq);
  first_work = (user + nice + system);
  fclose(fp);
  fp = NULL;

  sleep(1);
 
  fp = fopen("/proc/stat", "r");
  if (fp == NULL) {
    printf("error opening file in graphinfo\n");
    exit(0);
  }
  fscanf(fp, "%*s %Lf %Lf %Lf %Lf %Lf %Lf %Lf\n", &user, &nice, &system, &idle,
    &iowait, &irq, &softirq);
  second_total = (user + nice + system + idle + iowait + irq + softirq);
  second_work = (user + nice + system);
  fclose(fp);
  fp = NULL;

  whole_work = (second_work - first_work);
  whole_total = (second_total - first_total);
  cpu_percent = ((whole_work / whole_total) * 100);
  g_graph_info.cpu_usage = cpu_percent;
  rounded_percent = (int) ceil(cpu_percent);
  g_graph_info.cpu_round = rounded_percent;
}


void get_mem() {
  // get memory usage
  g_graph_info.mem_usage = 0;
  g_graph_info.mem_round = 0;
  FILE *fp = NULL;
  long double total_mem = 0;
  long double free_mem = 0;
  long double used_mem = 0;
  int rounded_percent = 0;

  fp = fopen("/proc/meminfo", "r");
  if (fp == NULL) {
    printf("error opening file in graphinfo\n");
    exit(0);
  }
  fscanf(fp, "%*s %Lf %*s\n", &total_mem);
  printf("got total mem: %Lf\n", total_mem);
  fscanf(fp, "%*s %Lf %*s\n", &free_mem);
  printf("got free_mem: %Lf\n", free_mem);
  used_mem = total_mem - free_mem;
  g_graph_info.mem_usage = used_mem;
  fclose(fp);
  fp = NULL;
  rounded_percent = (int) ceil(used_mem / total_mem);
  g_graph_info.mem_round = rounded_percent;
}


void get_swap() {
  // get swap usage
  g_graph_info.swap_usage = 0;
  g_graph_info.swap_round = 0;
  FILE *fp = NULL;
  long double total_swap = 0;
  long double free_swap = 0;
  long double used_swap = 0;
  int rounded_percent = 0;

  fp = fopen("/proc/meminfo", "r");
  if (fp == NULL) {
    printf("error opening file in graphinfo\n");
    exit(0);
  }

  for (int i = 0; i < 14; i++) {
    fscanf(fp, "%*s %Lf %*s\n", &total_swap);
  }
  fscanf(fp, "%*s %Lf %*s\n", &total_swap);
  fscanf(fp, "%*s %Lf %*s\n", &free_swap);
  used_swap = total_swap - free_swap;
  g_graph_info.swap_usage = used_swap;
  fclose(fp);
  fp = NULL;
  rounded_percent = (int) ceil(used_swap / total_swap);
  g_graph_info.swap_round = rounded_percent;
}


void get_network() {
  // get network usage
  g_graph_info.network_sent = 0;
  g_graph_info.network_recieved = 0;
  FILE *fp = NULL;
  long double sent = 0;
  long double recieved = 0;
  char line[1000];
  char interface[100];
  fp = fopen("/proc/net/dev", "r");
  if (fp == NULL) {
    printf("error opening file in graphinfo\n");
    exit(0);
  }
  fscanf(fp, "%[^\n]\n", line);   // skip first two lines in /proc/net/dev
  fscanf(fp, "%[^\n]\n", line);   // skip first two lines in /proc/net/dev
  
  while (fscanf(fp, "%s %Lf %*s %*s %*s %*s %*s %*s %*s %Lf %[^\n]", interface, &recieved, &sent, line)) {
    if (strcmp("eth0:", interface) == 0) {
      break;
    }
  }
  g_graph_info.network_sent = ceil(sent);
  g_graph_info.network_recieved = ceil(recieved);

  fclose(fp);
  fp = NULL;
}

graph_info get_graph_info() {
  get_cpu();
  get_mem();
  get_swap();
  get_network();

  return g_graph_info;
}
