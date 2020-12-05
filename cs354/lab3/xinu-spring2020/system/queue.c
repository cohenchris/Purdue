/* queue.c - enqueue, dequeue */

#include <xinu.h>

struct qentry	queuetab[NQENT];	/* Table of process queues	*/

/*------------------------------------------------------------------------
 *  enqueue  -  Insert a process at the tail of a queue
 *------------------------------------------------------------------------
 */
pid32	enqueue(
	  pid32		pid,		/* ID of process to insert	*/
	  qid16		q		/* ID of queue to use		*/
	)
{
	qid16	tail, prev;		/* Tail & previous node indexes	*/

	if (isbadqid(q) || isbadpid(pid)) {
		return SYSERR;
	}

	tail = queuetail(q);
	prev = queuetab[tail].qprev;

	queuetab[pid].qnext  = tail;	/* Insert just before tail node	*/
	queuetab[pid].qprev  = prev;
	queuetab[prev].qnext = pid;
	queuetab[tail].qprev = pid;

	return pid;
}

/*------------------------------------------------------------------------
 *  dequeue  -  Remove and return the first process on a list
 *------------------------------------------------------------------------
 */
pid32	dequeue(
	  qid16		q		/* ID queue to use		*/
	)
{
	pid32	pid;			/* ID of process removed	*/

	if (isbadqid(q)) {
		return SYSERR;
	} else if (isempty(q)) {
		return EMPTY;
	}

	pid = getfirst(q);
	queuetab[pid].qprev = EMPTY;
	queuetab[pid].qnext = EMPTY;

	return pid;
}

// Added cohen50 Lab 3

/*------------------------------------------------------------------------------
 * max_ready_val - get maximum value from ready list
 *------------------------------------------------------------------------------
 */
int32 max_ready_val(void) {
  if (isbadqid(readylist)) {
    return SYSERR;
  }

  int32 max_val;

  if (isempty(readylist)) {
    if (proctab[NULLPROC].prvgrosscpu == 0) {
      // Corner case for if nullproc prvgrosscpu is 0 so it doesn't underflow
      proctab[NULLPROC].prvgrosscpu++;
    }
    max_val = proctab[NULLPROC].prvgrosscpu - 1;
  }
  else {
    if (lastid(readylist) == NULLPROC) {
      if (queuetab[NULLPROC].qkey == 0) {
        // Corner case for if nullproc prvgrosscpu is 0 so it doesn't underflow
        proctab[NULLPROC].prvgrosscpu++;
        queuetab[NULLPROC].qkey++;
        max_val = lastkey(readylist) - 1;
      }
      else {
        max_val = lastkey(readylist) - 1;
      }
    }
    else {
      max_val = lastkey(readylist);
    }
  }

  /*
  kprintf("=====\n");
  print_ready_list();
  kprintf("max val = %d\n", max_val);
  kprintf("=====\n");
  */

  return max_val;
}

/*------------------------------------------------------------------------------
 * min_ready_val - get minimum value from ready list
 *------------------------------------------------------------------------------
 */
int32 min_ready_val(void) {
  if (isbadqid(readylist)) {
    return SYSERR;
  }

  int32 min_val;

  if (isempty(readylist)) {
    if (proctab[NULLPROC].prvgrosscpu == 0) {
      // Corner case for if nullproc prvgrosscpu is 0 so it doesn't underflow
      proctab[NULLPROC].prvgrosscpu++;
    }
    min_val = proctab[NULLPROC].prvgrosscpu - 1;
  }
  else {
    if (firstid(readylist) == NULLPROC) {
      if (queuetab[NULLPROC].qkey == 0) {
        proctab[NULLPROC].prvgrosscpu++;
        queuetab[NULLPROC].qkey++;
      }
      //min_val = firstkey(readylist) - 1;
      min_val = queuetab[firstid(q)
    }
    else {
      // Min val is the key of the first element in readylist
      min_val = firstkey(readylist);
    }
  }
/*
  kprintf("=====\n");
  print_ready_list();
  kprintf("min val = %d\n", min_val);
  kprintf("=====\n");
  */

  return min_val;
}

void print_ready_list() {
  kprintf("currently running: %s (%u) || HEAD  -->  ", proctab[currpid].prname,
      proctab[currpid].prvgrosscpu);
  qid16 curr = firstid(readylist);
  int32 currkey = queuetab[curr].qkey;
  while (curr != EMPTY && queuetab[curr].qnext != EMPTY) {
    kprintf("%s (%d)  -->  ", proctab[curr].prname, currkey);
    curr = queuetab[curr].qnext;
    currkey = queuetab[curr].qkey;
  }
  kprintf("TAIL\n");
}
