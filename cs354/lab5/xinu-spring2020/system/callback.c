/* callback.c - cbregister, cbunregister */

#include <xinu.h>

/*  CREATED BY CHRIS COHEN ON APRIL 7TH 2020 (Lab5 pt3)
 */

/*------------------------------------------------------------------------------
 * cbregister - registers a callback function for a process to be used when a
 *              message is received.
 *
 *              fp      = callback function pointer
 *              mbufptr = pointer to a 1-word message buffer
 *------------------------------------------------------------------------------
 */
syscall cbregister(void (* fp)(void), umsg32 *mbufptr) {
  intmask mask = disable();

  // SANITY CHECKS
  if (((uint32)fp >= (uint32)&etext - 1) ||     // fp is above the upper bound of text
      ((uint32)fp < (uint32)&text)) {           // fp is below the lower bound of text
    // Check if the function pointer is a valid function (if it's in the text section)
    restore(mask);
    return SYSERR;
  }
  else if (((uint32)mbufptr >= (uint32)&text) &&      // above the lower bound of text
           ((uint32)mbufptr <= (uint32)&etext - 1)) { // below the upper bound of text
    // Make sure that bufptr isn't a function (make sure it isn't in the text section)
    restore(mask);
    return SYSERR;
  }

  struct procent *prptr = &proctab[getpid()];

  // CHECK IF YOU CAN REGISTER A CALLBACK FUNCTION
  if (prptr->prcbvalid != 0) {
    // Valid callback function already registered, can't register more than one
    restore(mask);
    return SYSERR;
  }

  // No valid callback function registered --> able to register one
  prptr->prcbvalid = 1;         /* Register a valid callback function */
  prptr->prcbptr = fp;          /* fp is the callback function for the current proc */
  prptr->prmbufptr = mbufptr;   /* user-space bufptr for the callback function */

  restore(mask);
  return OK;
}

/*------------------------------------------------------------------------------
 * cbunregister - unregisters a process's callback function.
 *------------------------------------------------------------------------------
 */
syscall cbunregister(void) {
  intmask mask = disable();

  struct procent *prptr = &proctab[getpid()];

  // CHECK IF YOU CAN UNREGISTER A CALLBACK FUNCTION
  if (prptr->prcbvalid == 0) {
    // No valid callback function registered, so can't unregister one
    restore(mask);
    return SYSERR;
  }
  
  // Valid callback function is registered --> unregister it
  prptr->prcbvalid = 0;

  restore(mask);
  return OK;
}

/*------------------------------------------------------------------------------
 * timeslice_ipc - if there's a callback function and a waiting ipc message,
 *                 modify the runtime stack to detour to the callback function.
 *
 *                 This executes for case (ii), if the receiver had depleted its
 *                 whole timeslice.
 *------------------------------------------------------------------------------
 */
long timeslice_ipc(void) {
  struct procent * prptr = &proctab[getpid()];

  if ((!prptr->prcbvalid) || (!prptr->prtmpvalid)) {
    // If there isn't a callback function set up or there isn't a waiting
    // IPC message, don't do anything
    return NULL;
  }
  
  *prptr->prmbufptr = prptr->prtmpbuf;
  prptr->prtmpvalid = FALSE;

  return (long)prptr->prcbptr;
}
