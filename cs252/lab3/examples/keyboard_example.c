#include <unistd.h>
#include <stdio.h>
#include <string.h>

void tty_raw_mode();

/*
 *  Main function which goes into raw mode
 *  and prints what sequences it gets
 */

int main(int argc, char ** argv) {
  (void)(argc);
  (void)(argv);

  tty_raw_mode();

  while (1) {
    char ch = '\0';
    read(0, &ch, 1);
    printf("%d\n", ch);
  }
} /* main() */
