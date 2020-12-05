/* wgetprio.c - wgetprio */

#include <xinu.h>

/*
 * Wrapper function for system call getprio()
 * Created by cohen50 on 2-16-20
 */
syscall wgetprio(pid32 proc_pid) {
  /*
   * TODO:      assign system call number 5 to getprio() (store in EAX)
   *            execute int $33
   *            take return value of getprio() from EAX and return
   *            ** NOTE ** pass the argument 'proc_id' into EBX
   *            -- specified clobbered registers are EAX and EBX
   */

  syscall proc_prio = 0;
  int intr_num = 5;

  // Had to separate these asm() statements because it wouldn't let me mark
  // clobbered registers (EAX, EBX) when combined

  asm("movl %0, %%eax;"
      "movl %1, %%ebx;"
      : /* no output */
      : "g" (intr_num), "g" (proc_pid)
      : "%eax", "%ebx"
      );
  
  asm("int $33;"
      : "=a" (proc_prio)
      : /* no input */
      : /* no clobbered */
      );

  return proc_prio;
}
