/* testcpu.c - testcpu */

#include <xinu.h>

void testcpu(void) {
  int x, y;
  x = 0;
  y = clktimemilli;
  //kprintf("starting clktimemilli = %d\n", clktimemilli);

  while (clktimemilli - y < 8000) {
    // until wall clock time of 8 sec is reached do
    x++;
  }

  kprintf("cpu: %d %d %d %d\n", currpid, x, proctab[currpid].prgrosscpu,
                                procgrosscpumicro(currpid));
}
