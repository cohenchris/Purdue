/* bsend.c - bsend */

#include <xinu.h>

/*------------------------------------------------------------------------------
 * Blocking version of send() - if the receiver's buffer is full, block until
 * the receiver's buffer is free.
 *
 *                    Created by cohen50 for Lab4 on 3-10-20
 *------------------------------------------------------------------------------
 */
syscall bsend(pid32 pid, umsg32 msg) {
  intmask mask = disable();

  if (isbadpid(pid)) {
    restore(mask);
    return SYSERR;
  }

  if (proctab[currpid].prstate == PR_SLEEP) {
    /* Make sure that process isn't asleep. A process can only be present in one
     * queue at a time, and it will be taken out of the readylist later, so we
     * don't need to worry about that
     */

    restore(mask);
    return SYSERR;
  }
  
  struct procent *prptr = &proctab[pid];
  struct procent *currprptr = &proctab[currpid];
  
  if (prptr->prhasmsg) {
    /* If recipient has message, block until receiver's buffer is free */
    currprptr->prstate = PR_SND;
    currprptr->prsndmsg = msg;

    /* Make sure receiver knows it has messages */
    prptr->prsenderflag = TRUE;

    /* Add currpid into recipient's message queue to let recipient know which
     * process sent the message */
    enqueue(currpid, prptr->prblockedsenders);
  }
  else {
    /* If recipient doesn't have message, send it normally */
    send(pid, msg);
  }

  if (prptr->prstate == PR_RECV) {
    /* If the recipient is blocking in a receiving state, ready it up to receive
     * the new message
     */
    ready(pid);
  }
  else if (prptr->prstate == PR_RECTIM) {
    /* If the recipient is blocking in a timed receiving state, ready it up to
     * receive the new message
     */
    unsleep(pid);
    ready(pid);
  }

  resched();

  restore(mask);
  return OK;
}
