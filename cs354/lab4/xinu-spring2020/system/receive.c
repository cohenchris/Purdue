/* receive.c - receive */

#include <xinu.h>

/*------------------------------------------------------------------------
 *  receive  -  Wait for a message and return the message to the caller
 *------------------------------------------------------------------------
 */

umsg32	receive()
{
	intmask	mask;			/* Saved interrupt mask		*/
	struct	procent *prptr;		/* Ptr to process's table entry	*/
        struct  procent *sender;        /* Ptr to sender's table entry  */
	umsg32	msg;			/* Message to return		*/
        pid32   sender_pid;             /* pid of sending process       */

	mask = disable();
	prptr = &proctab[currpid];
        bool8   has_next_msg = FALSE;       /* does currpid have next msg?  */

        /* Can't receive a message if you don't have one! */
        if (!prptr->prhasmsg) {
          // Block until message arrives
          prptr->prstate = PR_RECV;
          resched();
        }

        /* If there is no next message, there were no queued messages */
        if (isempty(prptr->prblockedsenders)) {
          prptr->prsenderflag = FALSE;
        }

        /* Has queued sender(s) */
        if (prptr->prsenderflag) {
          // Extract next queued sender via dequeue()
          sender_pid = dequeue(prptr->prblockedsenders);

          // Unblock (ready up) sender process
          sender = &proctab[sender_pid];
          has_next_msg = TRUE;
          ready(sender_pid);
        }

	msg = prptr->prmsg;		        /* Retrieve message */
        //kprintf("received message - %u\n", msg);
	prptr->prhasmsg = has_next_msg;	        /* Reset message flag */

        if (has_next_msg) {
          // Get sender's message from its process table entry
          prptr->prmsg = sender->prsndmsg;
        }

	restore(mask);
	return msg;
}
