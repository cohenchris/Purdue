/* proclifetime.c - proclifetime */

#include <xinu.h>

/*
 * Returns number of ms since process creation.
 */
uint32 proclifetime(pid32 pid) {
  intmask mask = disable();

  if (isbadpid(pid)) {
    restore(mask);
    return SYSERR;
  }

  uint32 lifetime = clktimemilli - proctab[pid].prbirth;

  restore(mask);

  return lifetime;
}
