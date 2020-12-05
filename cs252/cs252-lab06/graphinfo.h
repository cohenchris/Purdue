#include <stdio.h>
#include <stdlib.h>

typedef struct {

  long double cpu_usage;
  long double mem_usage;
  long double swap_usage;
  long double network_sent;
  long double network_recieved;

  // rounded values to whole number 0 - 100

  int cpu_round;
  int mem_round;
  int swap_round;

} graph_info;

void get_cpu();
void get_mem();
void get_swap();
void get_network();
graph_info get_graph_info();
