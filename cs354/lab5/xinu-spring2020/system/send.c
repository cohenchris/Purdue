/* send.c - send */

#include <xinu.h>

/*------------------------------------------------------------------------
 *  send  -  Pass a message to a process and start recipient if waiting
 *------------------------------------------------------------------------
 */
syscall	send(
	  pid32		pid,		/* ID of recipient process	*/
	  umsg32	msg		  /* Contents of message		*/
	)
{
	intmask	mask;			        /* Saved interrupt mask		*/
	struct	procent *prptr;		/* Ptr to process's table entry	*/

	mask = disable();
	if (isbadpid(pid)) {
		restore(mask);
		return SYSERR;
	}

	prptr = &proctab[pid];

  /* Make sure you're able to send an async IPC message to the receiver */
  if (// receiver has callback, but already has an async IPC message waiting
      ((prptr->prcbvalid == TRUE) && (prptr->prtmpvalid == TRUE)) ||
      // receiver has no callback
      (prptr->prcbvalid == FALSE)) {
    // DISCARD
    restore(mask);
    return SYSERR;
  }

	prptr->prtmpbuf = msg;      /* Deliver message */
	prptr->prtmpvalid = TRUE;   /* Indicate message is waiting */

  /* MODIFY RUNTIME STACK OF RECEIVER */
  if (prptr->prstate == PR_SLEEP) {
    // case (i) - receiver is in a blocking state
    kprintf("SLEEPMS CASE:\n");
  }
  if (prptr->prstate == PR_READY) {
    // case (i) - receiver depleted its timeslice
    kprintf("TIMESLICE DEPLETED CASE:\n");
  }

	/* If recipient waiting or in timed-wait make it ready */
	if (prptr->prstate == PR_RECV) {
		ready(pid);
	} else if (prptr->prstate == PR_RECTIM) {
		unsleep(pid);
		ready(pid);
	}

	restore(mask);		/* Restore interrupts */
	return OK;
}
