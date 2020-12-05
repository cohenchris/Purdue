/* wgetppid.c - wgetppid */

#include <xinu.h>

/*
 * Wrapper function for system call getppid()
 * Created by cohen50 on 2-16-20
 */
pid32 wgetppid(pid32 proc_pid) {
  /*
   * TODO:      assign system call number 6 to getppid() (store in EAX)
   *            execute int $33
   *            take return value of getppid() from EAX and return
   *            ** NOTE ** pass the argument 'proc_pid' into EBX
   *            -- specified clobbered register is EAX
   */

  pid32 parent_id = 0;
  int intr_num = 6;     

  // Had to separate these asm() statements because it wouldn't let me mark
  // clobbered registers (EAX, EBX) when combined

  asm("movl %0, %%eax;"
      "movl %1, %%ebx;"
      : /* no output */
      : "g" (intr_num), "g" (proc_pid)
      : "%eax", "%ebx"
      );

  asm("int $33;"
      : "=a" (parent_id)
      : /* no input */
      : /* no clobbered */
      );

  return parent_id;
}
