/* memextract.c - memextract */

#include <xinu.h>

/*------------------------------------------------------------------------------
 * memextract - Deletes a list element from the in-use memblocks for the
 *              current process.
 *              hdptr  --> Header node of the in-use memblock list
 *              memptr --> Memory block to be freed
 *------------------------------------------------------------------------------
 */
void memextract(struct inusememblk *hdptr, char *memptr) {
  intmask mask = disable();

  struct inusememblk **curr = &hdptr;
  struct inusememblk *prev = NULL;

  while ((*curr != NULL) && ((*curr)->memblockptr != memptr)) {
    prev = *curr;
    *curr = (*curr)->mnext;
  }

  // Make sure block exists
  if ((*curr == NULL && prev == NULL) || (*curr)->memblockptr != memptr) {
    //kprintf("ERROR - no such memory block in the to-be-freed list for '%s'.\n", proctab[currpid].prname);
    return;
  }
  //kprintf("removing address %x from to-be-freed list for '%s'\n", memptr, proctab[currpid].prname);

  if (prev == NULL) {
    proctab[currpid].prheaphd = (*curr)->mnext;
  }
  else {
    prev->mnext = (*curr)->mnext;
  }
  // Signal to freemem() that it's being called from the kernel
  memblockkflag = 1;
  freemem((char *)(*curr), sizeof(struct inusememblk));

  restore(mask);
}
