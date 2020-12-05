/* resched.c - resched, resched_cntl */

#include <xinu.h>

struct	defer	Defer;

/*------------------------------------------------------------------------
 *  resched  -  Reschedule processor to highest priority eligible process
 *------------------------------------------------------------------------
 */
void	resched(void)		/* Assumes interrupts are disabled	*/
{
	struct procent *ptold;	/* Ptr to table entry for old process	*/
	struct procent *ptnew;	/* Ptr to table entry for new process	*/

	/* If rescheduling is deferred, record attempt and return */

	if (Defer.ndefers > 0) {
		Defer.attempt = TRUE;
		return;
	}

	/* Point to process table entry for the current (old) process */

	ptold = &proctab[currpid];

        ptold->prvgrosscpu += clktimemilli - currentgrosscpu;
        proctab[NULLPROC].prvgrosscpu += clktimemilli - currentgrosscpu;

	if (ptold->prstate == PR_CURR) {  /* Process remains eligible */
                // Edited by cohen50 from > to < for Lab 3
                // Also prprio changed to prvgrosscpu to help w/ fair scheduling
		//if (ptold->prprio > firstkey(readylist)) {
                if (ptold->prvgrosscpu < firstkey(readylist)) {
			return;
		}

		/* Old process will no longer remain current */

		ptold->prstate = PR_READY;
                // Added cohen50 Lab3 - adds to readylist by prvgrosscpu
                ///rinsert(currpid, readylist, ptold->prprio);
		rinsert(currpid, readylist, ptold->prvgrosscpu);
                // End added code cohen50 Lab3
	}

        // ADDED CODE BY cohen50 FOR Lab 3 

        if (clktimemilli == currentgrosscpu) {
          // Less than 1ms has passed for this proc, add 1ms to prgrosscpu
          ptold->prgrosscpu++;
          // Next line used to keep null process at lowest priority (highest CPU
          // use)
          proctab[NULLPROC].prvgrosscpu++;
        }
        else {
          ptold->prgrosscpu += clktimemilli - currentgrosscpu;
          // Next line used to keep null process at lowest priority (highest CPU
          // use)
          proctab[NULLPROC].prvgrosscpu += clktimemilli - currentgrosscpu;
        }

        uint64 tick_count = getticks();
        ptold->prgrosscputick += tick_count - currentgrosscputick;

        // END ADDED CODE BY cohen50 FOR Lab 3


	/* Force context switch to highest priority ready process */

	currpid = dequeue(readylist);
	ptnew = &proctab[currpid];
	ptnew->prstate = PR_CURR;
	preempt = QUANTUM;		/* Reset time slice for process	*/

	ctxsw(&ptold->prstkptr, &ptnew->prstkptr);

        // Added by cohen50 for Lab 3 to keep track of process CPU usage
        currentgrosscpu = clktimemilli;
        currentgrosscputick = getticks();

	/* Old process returns here when resumed */

	return;
}

/*------------------------------------------------------------------------
 *  resched_cntl  -  Control whether rescheduling is deferred or allowed
 *------------------------------------------------------------------------
 */
status	resched_cntl(		/* Assumes interrupts are disabled	*/
	  int32	defer		/* Either DEFER_START or DEFER_STOP	*/
	)
{
	switch (defer) {

	    case DEFER_START:	/* Handle a deferral request */

		if (Defer.ndefers++ == 0) {
			Defer.attempt = FALSE;
		}
		return OK;

	    case DEFER_STOP:	/* Handle end of deferral */
		if (Defer.ndefers <= 0) {
			return SYSERR;
		}
		if ( (--Defer.ndefers == 0) && Defer.attempt ) {
			resched();
		}
		return OK;

	    default:
		return SYSERR;
	}
}
