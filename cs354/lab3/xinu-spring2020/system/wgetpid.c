/* wgetpid.c - wgetpid */

#include <xinu.h>

/*
 * Wrapper function for system call getpid()
 * Created by cohen50 on 2-16-20
 */
pid32 wgetpid() {
  /*
   * TODO:      assign system call number 4 to getpid() (store in EAX)
   *            execute int $33
   *            take return value of getpid() from EAX and return
   *            -- specified clobbered register is EAX
   */

  pid32 proc_id;
  uint32 intr_num = 4;

  // Had to separate these asm() statements because it wouldn't let me mark
  // clobbered registers (EAX) when combined

  asm("movl %0, %%eax;"
      : /* no output */
      : "g" (intr_num)
      : "%eax"
     );

  asm("int $33;"
      : "=r" (proc_id)
      : /* no input */
      : /* no clobbered */
     );

  return proc_id;
}
