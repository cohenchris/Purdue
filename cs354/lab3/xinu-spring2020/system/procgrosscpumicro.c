/* procgrosscpumicro.c - procgrosscpumicro */

#include <xinu.h>

uint32 procgrosscpumicro(pid32 pid) {
  intmask mask = disable();

  if (isbadpid(pid)) {
    restore(mask);
    return SYSERR;
  }

  uint64 micro = proctab[pid].prgrosscputick * 2.5;  // tick time in ns
  micro = ((uint32) micro) / 1000;      // convert to time in ms (for 32 bit)

  restore(mask);
  return micro;
}
