#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <sys/resource.h>

#define TERMINAL 1

int main() {
  /* TODO: fork()
   * TODO: write() to terminal (file descriptor 1) after fork()
   * TODO: output the priority of the parent and child w/ getpriority()
   *
   * TODO: In the rcreate() version of create() in XINU, which process will run
   * first after rcreate() is called when the process running main() is created
   * in 3.3(a)?
   */

  int fork_return = fork();

  if (fork_return == 0) {
    // child process
    char * child_priority = (char *) malloc(sizeof(char) * 100);
    strncat(child_priority, "Child priority is ", 18);
    sprintf(child_priority + strlen(child_priority), "%d",
            getpriority(PRIO_PROCESS, getpid()));
    strncat(child_priority, "\n", 1);
    write(TERMINAL, child_priority, strlen(child_priority));
    free(child_priority);
    child_priority = NULL;
  }
  else if (fork_return > 0) {
    // parent process, child PID returned

    waitpid(fork_return, NULL, 0);
    // Find priority of parent process and write to terminal
    char * parent_priority = (char *) malloc(sizeof(char) * 100);
    strncat(parent_priority, "Parent priority is ", 19);
    sprintf(parent_priority + strlen(parent_priority), "%d",
            getpriority(PRIO_PROCESS, getpid()));
    strncat(parent_priority, "\n", 1);
    write(TERMINAL, parent_priority, strlen(parent_priority));
    free(parent_priority);
    parent_priority = NULL;

    /*
    // Find priority of child process and write to terminal
    char * child_priority = (char *) malloc(sizeof(char) * 100);
    strncat(child_priority, "Child priority is ", 18);
    sprintf(child_priority + strlen(child_priority), "%d",
            getpriority(PRIO_PROCESS, fork_return));
    strncat(child_priority, "\n", 1);
    write(TERMINAL, child_priority, strlen(child_priority));
    free(child_priority);
    child_priority = NULL;
    */

  }
  else { // fork_return < 0
    return -1;
    // fork() failed
  }
  return 0;
} /* parent_child() */
