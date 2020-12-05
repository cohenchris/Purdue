/* start_timestamp.c - start_timestamp */

#include <xinu.h>

void start_timestamp(void) {
  int32 mask = disable();

  // Start CPU ms timer for current process
  currentgrosscpu = clktimemilli;

  // Start CPU tick timer for current process
  currentgrosscputick = getticks();

  restore(mask);
  return;
}
