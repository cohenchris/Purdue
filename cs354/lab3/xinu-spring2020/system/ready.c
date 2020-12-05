/* ready.c - ready */

#include <xinu.h>

qid16	readylist;			/* Index of ready list		*/

/*------------------------------------------------------------------------
 *  ready  -  Make a process eligible for CPU service
 *------------------------------------------------------------------------
 */
status	ready(
	  pid32		pid		/* ID of process to make ready	*/
	)
{
	register struct procent *prptr;

	if (isbadpid(pid)) {
		return SYSERR;
	}

	/* Set process state to indicate ready and add to ready list */

	prptr = &proctab[pid];
        // Added cohen50 Lab 3 - prioritizes previously blocked processes
        //                       (sleepms()). If the process was asleep, make
        //                       its prvgrosscpu the minimum from the ready
        //                       list, then put it into the readylist as normal.
        if (prptr->prstate == PR_SLEEP) {
          // If the process was asleep, make prvgrosscpu the minimum from the
          // ready list in order to prioritize it. Minus one to make SURE it's
          // the first value, since it may get put further back if it has a key
          // equal to the first key.
          prptr->prvgrosscpu = min_ready_val() - 1;
        }

	prptr->prstate = PR_READY;
        // Added cohen50 Lab 3 - adds to readylist based on prvgrosscpu
	//rinsert(pid, readylist, prptr->prprio);
        rinsert(pid, readylist, prptr->prvgrosscpu);
        // End added code cohen50 for Lab 3
	resched();

	return OK;
}
