/* procgrosscpu.c - procgrosscpu */

#include <xinu.h>

uint32 procgrosscpu(pid32 pid) {
  intmask mask = disable();

  if (isbadpid(pid)) {
    restore(mask);
    return SYSERR;
  }

  uint32 grosscpu = proctab[pid].prgrosscpu;

  restore(mask);
  return grosscpu;
}
