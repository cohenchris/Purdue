/* rinsert.c - rinsert */

#include <xinu.h>

/*
 * Inserts processes in ascending order of priority into XINU's ready list
 */
status	rinsert(
	  pid32		pid,		/* ID of process to insert	*/
	  qid16		q,		/* ID of queue to use		*/
	  int32		key		/* Key for the inserted process	*/
	)
{
  if (isbadqid(q) || isbadpid(pid)) {
    return SYSERR;
  }


  qid16 curr;
  qid16 prev;

  curr = firstid(q);
  while ((queuetab[curr].qkey <= key) && (curr != NULLPROC) &&
         (curr != queuetail(q))) {
          curr = queuetab[curr].qnext;
  }

  /* Insert process between curr node and previous node */

  prev = queuetab[curr].qprev;	/* Get index of previous node	*/
  queuetab[pid].qnext = curr;
  queuetab[pid].qprev = prev;
  queuetab[pid].qkey = key;
  queuetab[prev].qnext = pid;
  queuetab[curr].qprev = pid;

  //kprintf("%s inserted\n", proctab[pid].prname);
  //print_ready_list();

  return OK;
}
