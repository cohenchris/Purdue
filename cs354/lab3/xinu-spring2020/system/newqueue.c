/* newqueue.c - newqueue */

#include <xinu.h>

/*------------------------------------------------------------------------
 *  newqueue  -  Allocate and initialize a queue in the global queue table
 *------------------------------------------------------------------------
 */
qid16	newqueue(void)
{
	static qid16	nextqid=NPROC;	/* Next list in queuetab to use	*/
	qid16		q;		/* ID of allocated queue 	*/

	q = nextqid;
	if (q >= NQENT) {		/* Check for table overflow	*/
		return SYSERR;
	}

	nextqid += 2;			/* Increment index for next call*/

	/* Initialize head and tail nodes to form an empty queue */

	queuetab[queuehead(q)].qnext = queuetail(q);
	queuetab[queuehead(q)].qprev = EMPTY;
        // Added by cohen50 for Lab 3 to make queue prioritize low values
	  //queuetab[queuehead(q)].qkey  = MAXKEY;
	  queuetab[queuehead(q)].qkey  = MINKEY;
        // End added code by cohen50 for Lab 3
	queuetab[queuetail(q)].qnext = EMPTY;
	queuetab[queuetail(q)].qprev = queuehead(q);
        // Added by cohen50 for Lab 3 to make queue prioritize low values
	  //queuetab[queuetail(q)].qkey  = MINKEY;
	  queuetab[queuetail(q)].qkey  = MAXKEY;
        // End added code by cohen50 for Lab 3
	return q;
}
