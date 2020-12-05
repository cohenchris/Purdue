/* gettmslice.h - gettmslice */

#include <xinu.h>

/*------------------------------------------------------------------------------
 * gettmslice - Checks how much time (in ms) remains in the current time slice
 * Created by Chris Cohen on 2-11-20
 *------------------------------------------------------------------------------
 */
uint32 gettmslice(void) {
  intmask mask;
  mask = disable;

  uint32 slice = preempt;

  restore(mask);
  return slice;
}
