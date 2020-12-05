/*  main.c  - main */

#include <xinu.h>

void receiver_proc(void);
void sender_proc(pid32, umsg32);
void victimA(void);

process	main(void)
{
        /* Removed default Hello World Message - Chris Cohen 1/26/2020
    	kprintf("\nHello World!\n");
    	kprintf("\nI'm the first XINU app and running function main() in system/main.c.\n");
    	kprintf("\nI was created by nulluser() in system/initialize.c using create().\n");
    	kprintf("\nMy creator will turn itself into the do-nothing null process.\n");
    	kprintf("\nI will create a second XINU app that runs shell() in shell/shell.c as an example.\n");
    	kprintf("\nYou can do something else, or do nothing; it's completely up to you.\n");
    	kprintf("\n...creating a shell\n");
        */

	/* Run the Xinu shell */

        // Added by Chris Cohen 1/28/20
        //int x = 3 / 0;  // Divide by zero to test interrupts

  /*
	recvclr();
	resume(create(shell, 8192, 50, "shell", 1, CONSOLE));

	// Wait for shell to exit and recreate it

	while (TRUE) {
		receive();
		sleepms(200);
		kprintf("\n\nMain process recreating shell\n\n");
		resume(create(shell, 4096, 20, "shell", 1, CONSOLE));
	}
    */

        /////////////////// TESTS FOR PART 3 //////////////////////
        pid32 receiver_pid = create(receiver_proc, 8192, 10, "receiver", 0, NULL);
        resume(receiver_pid);

        resume(create(sender_proc, 8192, 10, "sender1", 2, receiver_pid, 69));
        resume(create(sender_proc, 8192, 10, "sender2", 2, receiver_pid, 70));
        resume(create(sender_proc, 8192, 10, "sender3", 2, receiver_pid, 71));
        resume(create(sender_proc, 8192, 10, "sender4", 2, receiver_pid, 72));
        resume(create(sender_proc, 8192, 10, "sender5", 2, receiver_pid, 73));
        /////////////////// END TESTS FOR PART 3 //////////////////

        /////////////////// TESTS FOR PART 4 //////////////////////
        //resume(create(victimA, 8192, 10, "victimA_1", 0, NULL));
        //resume(create(victimA, 8192, 10, "victimA_2", 0, NULL));
        //resume(create(victimA, 8192, 10, "victimA_3", 0, NULL));

        // victimA_1 shouldn't print the second time - hijacked
        //resume(create(attackerA, 8192, 10, "attackerA", 1, 3));

        // victimA_1's x-value should be 9 now, runs as normal
        //resume(create(attackerB, 8192, 10, "attackerB", 1, 3));
        /////////////////// END TESTS FOR PART 4 ///////////////////

	return OK;
}

void receiver_proc(void) {
  while (TRUE) {
    receive();
    sleepms(200);
  }
}

void sender_proc(pid32 pid, umsg32 send_msg) {
  bsend(pid, send_msg);
}

void victimA(void) {
  int x;

  x = 5;
  kprintf("before funcA: %d %d\n", getpid(), x);
  sleepms(300);
  kprintf("after funcA: %d %d\n", getpid(), x);
}
