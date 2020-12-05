#include <stdio.h>
#include <string.h>
#include <unistd.h>

char *read_line();
char *g_prompt = "myshell>";

/*
 *  Main function which prints a shell prompt, then uses your read_line
 *  implementation to read a line of input and print it
 */

int main(int argc, char ** argv) {
  (void)(argc);
  (void)(argv);
  
  // Example to test read_line
  while (1) {

    char * s = NULL;

    // Print prompt
    write(1, g_prompt, strlen(g_prompt));

    // Read one line
    s = read_line();

    // Print line read
    printf("line read: %s\n", s);
  }
} /* main() */
