/*  main.c  - main */

#include <xinu.h>

void    sndch(char);
void    test_main(void);

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

        /*      Commented out by Chris Cohen 2-11-20
	recvclr();
	resume(create(shell, 8192, 50, "shell", 1, CONSOLE));

	// Wait for shell to exit and recreate it

	while (TRUE) {
		receive();
		sleepms(200);
		kprintf("\n\nMain process recreating shell\n\n");
		resume(create(shell, 4096, 20, "shell", 1, CONSOLE));
	}
	return OK;
                End Commented Out Section by Chris Cohen 2-11-20 */

        // TEST CODE 2-16-20 FOR LAB 2
        // Written by cohen50 on 2-16-20
        pid32 no_w_pid = getpid();
        kprintf("pid of no wrapper current process =    %d\n", no_w_pid);
        pid32 curr_pid = wgetpid();
        kprintf("pid of current process =               %d\n\n", curr_pid);

        //pid32 no_w_parent = getppid(currpid);
        pid32 no_w_parent = getppid(wgetpid());
        kprintf("parent pid of no wrapper curr proc =   %d\n", no_w_parent);
        //pid32 parent_pid = wgetppid(currpid);
        pid32 parent_pid = wgetppid(wgetpid());
        kprintf("parent pid of current process =        %d\n\n", parent_pid);

        //syscall no_w_proc_prio = getprio(currpid);
        syscall no_w_proc_prio = getprio(wgetpid());
        kprintf("priority of no wrapper curr proc =     %d\n", no_w_proc_prio);
        //syscall proc_prio = wgetprio(currpid);
        syscall proc_prio = wgetprio(wgetpid());
        kprintf("priority of current process =          %d\n\n", proc_prio);

        syscall no_w_par_prio = getprio(no_w_parent);
        kprintf("priority of no wrapper parent =        %d\n", no_w_par_prio);
        syscall parent_prio = wgetprio(no_w_parent);
        kprintf("priority of parent process =           %d\n\n", parent_prio);

	rcreate(test_main, 4096, 30, "test_main", 0);
        // END TEST CODE 2-16-20 FOR LAB 2
        while(1);
}

void test_main() {
        pid32 no_w_pid = getpid();
        kprintf("pid of no wrapper current process =    %d\n", no_w_pid);
        pid32 curr_pid = wgetpid();
        kprintf("pid of current process =               %d\n\n", curr_pid);

        //pid32 no_w_parent = getppid(currpid);
        pid32 no_w_parent = getppid(wgetpid());
        kprintf("parent pid of no wrapper curr proc =   %d\n", no_w_parent);
        //pid32 parent_pid = wgetppid(currpid);
        pid32 parent_pid = wgetppid(wgetpid());
        kprintf("parent pid of current process =        %d\n\n", parent_pid);

        //syscall no_w_proc_prio = getprio(currpid);
        syscall no_w_proc_prio = getprio(wgetpid());
        kprintf("priority of no wrapper curr proc =     %d\n", no_w_proc_prio);
        //syscall proc_prio = wgetprio(currpid);
        syscall proc_prio = wgetprio(wgetpid());
        kprintf("priority of current process =          %d\n\n", proc_prio);

        syscall no_w_par_prio = getprio(no_w_parent);
        kprintf("priority of no wrapper parent =        %d\n", no_w_par_prio);
        syscall parent_prio = wgetprio(no_w_parent);
        kprintf("priority of parent process =           %d\n\n", parent_prio);
}
