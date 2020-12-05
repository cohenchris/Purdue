#include <stdio.h>

#include "test_funcs.h"
#include "my_malloc.h"
#include "printing.h"

#define NUM_INTS (8)

/*
 * Contains an allocation pattern that will discern between
 * the fit algorithms (with the exception of next_fit)
 */

int main()
{
  int * arr[NUM_INTS];

  arr[0] = (int *) my_malloc(100 * sizeof(int));
  arr[1] = (int *) my_malloc(5 * sizeof(int));
  arr[2] = (int *) my_malloc(10 * sizeof(int));
  arr[3] = (int *) my_malloc(5 * sizeof(int));
  arr[4] = (int *) my_malloc(10 * sizeof(int));
  arr[5] = (int *) my_malloc(5 * sizeof(int));
  arr[6] = (int *) my_malloc(100 * sizeof(int));
  arr[7] = (int *) my_malloc(5 * sizeof(int));

  //  Chunk up memory
  for (int i = 0; i < NUM_INTS; i++) {
    if (i % 2 == 0) {
      my_free(arr[i]);
    }
  }

  freelist_print(print_object);
  printf("BREAK\n");

  arr[0] = (int *) my_malloc(10 * sizeof(int));

  freelist_print(print_object);
  printf("BREAK\n");

  arr[2] = (int *) my_malloc(10 * sizeof(int));

  freelist_print(print_object);
  printf("BREAK\n");

  return 0;
} /* main() */
