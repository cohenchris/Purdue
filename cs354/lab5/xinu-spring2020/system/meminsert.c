/* meminsert.c - meminsert */

#include <xinu.h>

/*------------------------------------------------------------------------------
 * meminsert - Inserts a memory block into the in-use memblocks for the current
 *             process. Elements are sorted in increasing order of mem address.
 *             hdptr  --> Header node of the in-use memblock list
 *             memptr --> Memory block to be inserted
 *------------------------------------------------------------------------------
 */
void meminsert(struct inusememblk *hdptr, char *memptr) {
  intmask mask = disable();
  //kprintf("inserting address %x for '%s' with size %u\n", memptr, proctab[currpid].prname, ((struct memblk *)(memptr))->mlength);

  // For memory allocation, signal to getmem() that this IS a kernel call
  memblockkflag = 1;
  struct inusememblk *new = (struct inusememblk *)getmem(sizeof(struct inusememblk));
  new->memblockptr = memptr;
  new->mnext = NULL;

  // Loop until you get to the element that you should insert at
  struct inusememblk ** curr = &hdptr;
  if ((*curr == NULL) || ((*curr)->memblockptr >= memptr)) {
    // Checks corner case for the 1st node. Makes the rest easier.
    new->mnext = *curr;
    *curr = new;
    proctab[currpid].prheaphd = *curr;
    return;
  }

  // Loop through until you find the element to insert before
  while (((*curr)->mnext != NULL) &&
         ((*curr)->mnext->memblockptr < memptr)) {
    *curr = (*curr)->mnext;
  }
  new->mnext = (*curr)->mnext;
  (*curr)->mnext = new;

  restore(mask);
}

void print_freelist(pid32 pid) {
  intmask mask = disable();
  struct inusememblk *tmp = proctab[pid].prheaphd;
  /*
  if (isbadpid(pid)) {
    kprintf("bad pid for print_freelist()\n");
    return;
  }
  */

  uint32 length;
  kprintf("------------------------------\n");
  while (tmp != NULL) {
    length = ((struct memblk *)(tmp->memblockptr))->mlength;
    kprintf("memblockptr = %x\t%uMb\n", tmp->memblockptr, length);
    tmp = tmp->mnext;
  }
  kprintf("------------------------------\n");
  restore(mask);
}
