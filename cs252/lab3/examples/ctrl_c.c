
//
// Example of how to ignore ctrl-c
//

#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <stdlib.h>

/*
 *  Display signal number which was called
 */

extern void disp(int sig) {
  fprintf(stderr, "\nsig: %d\n", sig);
} /* disp() */

/*
 *  Main function which registers a sigaction for SIGINT
 */

int main() {
  printf("Type ctrl-c or \"exit\"\n");

  struct sigaction sa = {0};
  sa.sa_handler = disp;
  sigemptyset(&sa.sa_mask);
  sa.sa_flags = 0;

  if (sigaction(SIGINT, &sa, NULL)) {
    perror("sigaction");
    exit(2);
  }

  for (;;) {
    char s[20];
    s[19] = '\0';

    printf("prompt>");
    fflush(stdout);

    fgets(s, 19, stdin);

    if (!strcmp(s, "exit\n")) {
      printf("Bye!\n");
      exit(1);
    }
  }

  return 0;
} /* main() */


