/* getmem.c - getmem */

#include <xinu.h>

/*------------------------------------------------------------------------
 *  getmem  -  Allocate heap storage, returning lowest word address
 *------------------------------------------------------------------------
 */
char  	*getmem(
	  uint32	nbytes		/* Size of memory requested	*/
	)
{
	intmask	mask;			/* Saved interrupt mask		*/
	struct	memblk	*prev, *curr, *leftover;

	mask = disable();
	if (nbytes == 0) {
    memblockkflag = 0;
		restore(mask);
		return (char *)SYSERR;
	}

	nbytes = (uint32) roundmb(nbytes);	/* Use memblk multiples	*/

	prev = &memlist;
	curr = memlist.mnext;
	while (curr != NULL) {			/* Search free list	*/

		if (curr->mlength == nbytes) {	/* Block is exact match	*/
			prev->mnext = curr->mnext;
			memlist.mlength -= nbytes;

      // CHRIS COHEN LAB 5 - GARBAGE COLLECTION
      // Update this field for later use
      curr->mlength = nbytes;
      // Add memblk to current process's to-be-freed list (if getmem() was called by app code)
      if (memblockkflag == 0) {
        //kprintf("inserting %u bytes into list\n", nbytes);
        meminsert(proctab[currpid].prheaphd, (char *)curr);
      }
      memblockkflag = 0;
      // END ADDED CODE FOR LAB 5

			restore(mask);
			return (char *)(curr);

		} else if (curr->mlength > nbytes) { /* Split big block	*/
			leftover = (struct memblk *)((uint32) curr +
					nbytes);
			prev->mnext = leftover;
			leftover->mnext = curr->mnext;
			leftover->mlength = curr->mlength - nbytes;
			memlist.mlength -= nbytes;

      // CHRIS COHEN LAB 5 - GARBAGE COLLECTION
      // Update this field for later use
      curr->mlength = nbytes;
      // Add memblk to current process's to-be-freed list (if getmem() was called by app code)
      if (memblockkflag == 0) {
        //kprintf("inserting %u bytes into list\n", nbytes);
        meminsert(proctab[currpid].prheaphd, (char *)curr);
      }
      memblockkflag = 0;
      // END ADDED CODE FOR LAB 5

			restore(mask);
			return (char *)(curr);
		} else {			/* Move to next block	*/
			prev = curr;
			curr = curr->mnext;
		}
	}
  memblockkflag = 0;
	restore(mask);
	return (char *)SYSERR;
}
