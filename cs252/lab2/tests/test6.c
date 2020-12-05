#include <stdio.h>

#include "my_malloc.h"
#include "printing.h"

#define NUM_INTS (4)

/*
 * Test that the correct fencepost blocks
 * are created for a simple job (malloc & free)
 * (the diff version of 4)
 */

int main()
{
  int * arr[NUM_INTS];

  for (int i = 0; i < NUM_INTS; i++) {
    arr[i] = (int *) my_malloc(16 * sizeof(int));
    freelist_print(print_object);
  }
  printf("BREAK\n");

  for (int i = 0; i < NUM_INTS - 1; i++) {
    my_free(arr[i]);
    freelist_print(print_object);
  }
  printf("BREAK\n");
  my_free(arr[NUM_INTS - 1]);
  freelist_print(print_object);

  printf("BREAK\n");

  return 0;
} /* main() */
