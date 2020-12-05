/* getppid.c - getppid */

#include <xinu.h>

/*------------------------------------------------------------------------
 *  getppid  -  Returns the PID argument's parent PID
 *  Created by Chris Cohen 2-11-20
 *------------------------------------------------------------------------
 */
pid32	getppid(pid32 curr_pid)
{
        intmask mask;
        mask = disable();

        if (isbadpid(curr_pid) || curr_pid <= 0) {
          restore(mask);
          return SYSERR;
        }

        pid32 parent_pid = proctab[curr_pid].prparent;

        restore(mask);

        return parent_pid;
}
