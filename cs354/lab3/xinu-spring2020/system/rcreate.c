#include <xinu.h>

// FILE CREATED BY Chris Cohen on 1/26/2020

/*-----------------------------------------------------------------------------
 *  rcreate  -  Create and resume a process to start running a function on x86
 *-----------------------------------------------------------------------------
 */
pid32 rcreate(
              void          *funcaddr,      /* Address of the function      */
              uint32        ssize,          /* Stack size in bytes          */
              pri16         priority,       /* Process priority > 0         */
              char          *name,          /* Name (for debugging)         */
              uint32        nargs,          /* Number of args that follow   */
              ...
              )
{
        intmask mask = disable();
        pid32 pid = create(funcaddr, ssize, priority, name, nargs);
        ready(pid);
        restore(mask);
        return pid;

} /* rcreate() */
