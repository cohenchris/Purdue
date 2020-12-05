/* procbirth.c - procbirth */

#include <xinu.h>

/*
 * Returns time (in ms) that process was created.
 */
uint32 procbirth(pid32 pid) {
  intmask mask = disable();

  if (isbadpid(pid)) {
    restore(mask);
    return SYSERR;
  }

  uint32 birth = proctab[pid].prbirth;

  restore(mask);

  return birth;
}
