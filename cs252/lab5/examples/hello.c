#include <stdio.h>

#define LF (10)
#define CR (13)

/*
 * Generate a simple hello, world response
 */

extern void httprun(int ssock, char *query_string) {
  // Open sub socket for printfing
  FILE *fssock = fdopen(ssock, "r+");
  if (fssock == NULL) {
    perror("fdopen");
  }

  fprintf(fssock, "Content-type: text/html%c%c%c%c", CR, LF, CR, LF);
  
  fprintf(fssock, "<h1>Hello. Hi from hello.so</h1>\n");

  // Close sub socket
  fclose(fssock);
}

