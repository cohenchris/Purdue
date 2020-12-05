/* wakeup.c - wakeup */

#include <xinu.h>

/*------------------------------------------------------------------------
 *  wakeup  -  Called by clock interrupt handler to awaken processes
 *------------------------------------------------------------------------
 */
void	wakeup(void)
{
	/* Awaken all processes that have no more time to sleep */

	resched_cntl(DEFER_START);
	while (nonempty(sleepq) && (firstkey(sleepq) <= 0)) {
          /*
                pid = dequeue(sleepq);
                kprintf("after waking %s\n", proctab[pid].prname);
                print_ready_list();
                ready(pid);
                */
                ready(dequeue(sleepq));
	}

	resched_cntl(DEFER_STOP);
	return;
}
