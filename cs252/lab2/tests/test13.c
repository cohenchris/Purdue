#include <stdio.h>

#include "test_funcs.h"
#include "my_malloc.h"
#include "printing.h"

#define NUM_INTS (6)

/*
 * Tests next_fit:
 *  -do an allocation where the program needs to loop back and start searching
 *   from root as well
 *  -ensure their search logic is correct
 */

int main()
{
  int * arr[NUM_INTS];

  arr[0] = (int *) my_malloc(ARENA_SIZE / 2);
  arr[1] = (int *) my_malloc(5 * sizeof(int));
  arr[2] = (int *) my_malloc(100 * sizeof(int));
  arr[3] = (int *) my_malloc(5 * sizeof(int));
  arr[4] = (int *) my_malloc(10 * sizeof(int));
  arr[5] = (int *) my_malloc(5 * sizeof(int));

  //  Chunk up memory
  for (int i = NUM_INTS - 1; i > 0; i--) {
    if (i % 2 == 0) {
      my_free(arr[i]);
    }
  }

  freelist_print(print_object);
  printf("BREAK\n");

  //  Grab the medium sized block, next should now grab small
  arr[4] = (int *) my_malloc(100 * sizeof(int));
  my_free(arr[0]);  //  Put large block in at front

  freelist_print(print_object);
  printf("BREAK\n");

  //  This should grab the small block
  arr[0] = (int *) my_malloc(10 * sizeof(int));
  freelist_print(print_object);
  printf("BREAK\n");

  //  This should grab the large block after looping back around
  arr[2] = (int *) my_malloc(10 * sizeof(int));
  freelist_print(print_object);
  printf("BREAK\n");


  return 0;
} /* main() */
