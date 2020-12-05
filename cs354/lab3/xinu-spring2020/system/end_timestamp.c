/* end_timestamp.c - end_timestamp */

#include <xinu.h>

void end_timestamp(void) {
  int32 mask = disable();

  // Update ms CPU usage for current process
  if (clktimemilli == currentgrosscpu) {
    // Less than 1ms has passed for this proc, add 1ms to prgrosscpu/prvgrosscpu
    proctab[currpid].prgrosscpu++;
    proctab[currpid].prvgrosscpu++;

    // Next line used to keep null process at lowest priority (highest CPU use),
    // and update queuetab table entry

    proctab[NULLPROC].prvgrosscpu++;
  }
  else {
    proctab[currpid].prgrosscpu += clktimemilli - currentgrosscpu;
    proctab[currpid].prvgrosscpu += clktimemilli - currentgrosscpu;
    // Next line used to keep null process at lowest priority (highest CPU use),
    // and update queuetab table entry
    proctab[NULLPROC].prvgrosscpu += clktimemilli - currentgrosscpu;
  }

  // Update tick CPU usage for current process
  uint64 tick_count = getticks();
  proctab[currpid].prgrosscputick += tick_count - currentgrosscputick;

  // Reset timers since they're shared between processes
  currentgrosscpu = clktimemilli;
  currentgrosscputick = getticks();

  restore(mask);
  return;
}
