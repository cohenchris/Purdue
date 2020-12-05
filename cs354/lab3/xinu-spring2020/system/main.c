/*  main.c  - main */

#include <xinu.h>

void print_ready_list(void);
process sndA(void);
process sndB(void);
process test_proc(void);

process	main(void)
{
  /*------------------------- ORIGINAL SHELL PROCESS ---------------------------
	recvclr();
	resume(create(shell, 8192, 50, "shell", 1, CONSOLE));
	while (TRUE) {
		receive();
		sleepms(200);
		kprintf("\n\nMain process recreating shell\n\n");
		resume(create(shell, 4096, 20, "shell", 1, CONSOLE));
	}
	return OK;
   *///-------------------------------------------------------------------------

  /*
  resume(create(sndA, 4096, 50, "sndA", 0, NULL));
  resume(create(sndB, 4096, 35, "sndB", 0, NULL));
  resume(create(sndA, 4096, 45, "sndA_2", 0, NULL));
  resume(create(sndB, 4096, 25, "sndB_2", 0, NULL));
  resume(create(test_proc, 4096, 30, "test", 0, NULL));
  resume(create(test_proc, 4096, 40, "test_2", 0, NULL));

  kprintf("\n\n\n%s done\n\n\n", proctab[currpid].prname);
  */


  // 5.2 - TEST SCENARIO A
  /*
  rcreate(testcpu, 4096, 30, "testcpu1", 0, NULL);
  rcreate(testcpu, 4096, 30, "testcpu2", 0, NULL);
  rcreate(testcpu, 4096, 30, "testcpu3", 0, NULL);
  rcreate(testcpu, 4096, 30, "testcpu4", 0, NULL);
  */

  // 5.3 - TEST SCENARIO B
  /*
  rcreate(testio, 4096, 30, "testio1", 0, NULL);
  rcreate(testio, 4096, 30, "testio2", 0, NULL);
  rcreate(testio, 4096, 30, "testio3", 0, NULL);
  rcreate(testio, 4096, 30, "testio4", 0, NULL);
  */

  // 5.4 - TEST SCENARIO C
  /*
  rcreate(testcpu, 4096, 30, "testcpu1", 0, NULL);
  rcreate(testcpu, 4096, 30, "testcpu2", 0, NULL);
  rcreate(testcpu, 4096, 30, "testcpu3", 0, NULL);
  rcreate(testcpu, 4096, 30, "testcpu4", 0, NULL);
  rcreate(testio, 4096, 30, "testio1", 0, NULL);
  rcreate(testio, 4096, 30, "testio2", 0, NULL);
  rcreate(testio, 4096, 30, "testio3", 0, NULL);
  rcreate(testio, 4096, 30, "testio4", 0, NULL);
  */

  // 5.5 - TEST SCENARIO D
  rcreate(testcpu, 4096, 30, "testcpu1", 0, NULL);
  sleepms(500);
  rcreate(testcpu, 4096, 30, "testcpu2", 0, NULL);
  sleepms(500);
  rcreate(testcpu, 4096, 30, "testcpu3", 0, NULL);
  sleepms(500);
  rcreate(testcpu, 4096, 30, "testcpu4", 0, NULL);
  
  //kprintf("\n\n\n%s done\n\n\n", proctab[currpid].prname);
  return OK;
}

process sndA(void) {
  sleepms(2000);
  /*
  while (TRUE) {
    kprintf("A");
  }
  */
  kprintf("\n\n\n%s done\n\n\n", proctab[currpid].prname);
  return OK;
}

process sndB(void) {
  sleepms(2000);
  /*
  while (TRUE) {
    kprintf("B");
  }
  */
  kprintf("\n\n\n%s done\n\n\n", proctab[currpid].prname);
  return OK;
}

process test_proc(void) {
  sleepms(2000);
  kprintf("\n\n\n%s done\n\n\n", proctab[currpid].prname);
  return OK;
}
