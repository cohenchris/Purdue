/*  main.c  - main */

#include <xinu.h>

process sleep_receiver(void);
process timeslice_receiver(void);
void callback_test(void);
process sender(pid32, pid32);
process test(void);

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

  // PART 3 TESTS
  /*
  pid32 pid1 = create(sleep_receiver, 4096, 20, "sleep_receiver", 0, NULL);
  pid32 pid2 = create(timeslice_receiver, 4096, 20, "timeslice_receiver", 0, NULL);
  resume(pid1);
  resume(pid2);
  resume(create(sender, 4096, 20, "sender", 2, pid1, pid2));
  */

  // PART 4 TESTS
  pid32 test_pid = create(test, 4096, 20, "test", 0, NULL);
  resume(test_pid);

  sleepms(200);

  //kprintf("FREELIST AFTER TEST DONE\n");
  //print_freelist(test_pid);

	return OK;
}

process test() {
  char * data1 = getmem(69);
  //print_freelist(currpid);

  char * data2 = getmem(420);
  //print_freelist(currpid);

  //kprintf("REMOVING %x\n", data1);
  freemem(data1, 69);
  //print_freelist(currpid);

  char * data3 = getmem(6969);
  //print_freelist(currpid);

  char * data4 = getmem(1234);
  //print_freelist(currpid);

  char * data5 = getmem(3535);
  //print_freelist(currpid);

  char * data6 = getmem(2424);
  //print_freelist(currpid);

  char * data7 = getmem(8802);
  //print_freelist(currpid);

  //kprintf("REMOVING %x\n", data4);
  freemem(data4, 1234);
  //print_freelist(currpid);

  data4 = getmem(1234);
  //print_freelist(currpid);

  //kprintf("REMOVING %x\n", data6);
  freemem(data6, 2424);
  //print_freelist(currpid);

  return OK;
}

process sleep_receiver() {
  struct procent * prptr = &proctab[getpid()];

  // Register callback function
  if (cbregister(&callback_test, &prptr->prmsg) != OK) {
     kprintf("registration of %x failed\n", (uint32) callback_test);
     userret();        /* proceeding with app code is nonsensical */
  }

  sleepms(500);

  return OK;
}

process timeslice_receiver() {
  struct procent * prptr = &proctab[getpid()];

  // Register callback function
  if (cbregister(&callback_test, &prptr->prmsg) != OK) {
     kprintf("registration of %x failed\n", (uint32) callback_test);
     userret();        /* proceeding with app code is nonsensical */
  }

  for (int i = 0; i < 10000000; i++);

  return OK;
}

process sender(pid32 pid1, pid32 pid2) {
  send(pid1, 24);
  send(pid2, 69);
  return OK;
}

void callback_test() {
  struct procent * prptr = &proctab[getpid()];

  kprintf("callback msg for %s = %d\n", prptr->prname, prptr->prmsg);
}
