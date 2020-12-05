/* sleep.c - sleep sleepms */

#include <xinu.h>

#define	MAXSECONDS	2147483		/* Max seconds per 32-bit msec	*/

/*------------------------------------------------------------------------
 *  sleep  -  Delay the calling process n seconds
 *------------------------------------------------------------------------
 */
syscall	sleep(
	  int32	delay		/* Time to delay in seconds	*/
	)
{
	if ( (delay < 0) || (delay > MAXSECONDS) ) {
		return SYSERR;
	}
	sleepms(1000*delay);
	return OK;
}

/*------------------------------------------------------------------------
 *  sleepms  -  Delay the calling process n milliseconds
 *------------------------------------------------------------------------
 */
syscall	sleepms(
	  int32	delay			/* Time to delay in msec.	*/
	)
{
	intmask	mask;			/* Saved interrupt mask		*/

  struct procent *prptr = &proctab[currpid];

	if (delay < 0) {
		return SYSERR;
	}

	if (delay == 0) {
		yield();
		return OK;
	}

	/* Delay calling process */

	mask = disable();
	if (insertd(currpid, sleepq, delay) == SYSERR) {
		restore(mask);
		return SYSERR;
	}

	prptr->prstate = PR_SLEEP;
	resched();

  /* Check if a callback needs to be handled */

  if ((!prptr->prcbvalid) || (!prptr->prtmpvalid)) {
    // If there isn't a callback function set up or there isn't a waiting
    // IPC message, don't do anything
    restore(mask);
    return OK;
  }

  // There IS a callback function set up, so deal accordingly

  // Put the message in the user-space buffer
  *prptr->prmbufptr = prptr->prtmpbuf;
  prptr->prtmpvalid = FALSE;

  // Modify the runtime stack to detour to the callback function

  uint32 *stkptr = NULL;
  asm("movl %%esp, %0;"
      : "=r"(stkptr)
      : // no input registers
      : // no clobbered registers
     );

  // Make the program jump to the callback first, then the original return address
  uint32 original_ret_addr = *(stkptr + 7);
  *(stkptr + 7) = (long)*prptr->prcbptr;
  *(stkptr + 8) = (long)original_ret_addr;

	restore(mask);
	return OK;
}
