#include <stdio.h>

#include "test_funcs.h"
#include "my_malloc.h"
#include "printing.h"

#define NUM_INTS (8)

/*
 * Tests next_fit:
 *  -do an allocation where a block pointed to by g_next_allocate gets
 *  coalesced
 */

int main()
{
  int * arr[NUM_INTS];


  arr[0] = (int *) my_malloc(10 * sizeof(int));
  arr[1] = (int *) my_malloc(10 * sizeof(int));
  arr[2] = (int *) my_malloc(10 * sizeof(int));
  arr[3] = (int *) my_malloc(10 * sizeof(int));
  arr[4] = (int *) my_malloc(100 * sizeof(int));
  arr[5] = (int *) my_malloc(10 * sizeof(int));
  arr[6] = (int *) my_malloc(10 * sizeof(int));

  //  Allocate (effectively) the rest of the space so g_next_allocate doesn't
  //  perpetually point to the remainder block
  int remaining_space = ARENA_SIZE - 6 * 10 * sizeof(int) - 100 * sizeof(int)
                        - 10 * ALLOC_HEADER_SIZE - 4 * sizeof(header);
  arr[7] = (int *) my_malloc(remaining_space);

  //  Chunk up memory
  for (int i = 0; i < NUM_INTS; i++) {
    if (i % 2 == 0) {
      my_free(arr[i]);
    }
  }

  freelist_print(print_object);
  printf("BREAK\n");


  arr[6] = (int *) my_malloc(100 * sizeof(int));
  //  The next to allocate should now be arr[2]
  
  assert(g_next_allocate == (header *) (((char *)arr[2]) - ALLOC_HEADER_SIZE));

  freelist_print(print_object);
  printf("BREAK\n");

  //  This should coalesce with arr[2] (and arr[0]), putting the next allocate
  //  at arr[0]
  my_free(arr[1]);
  assert(g_next_allocate == (header *) (((char *)arr[0]) - ALLOC_HEADER_SIZE));

  freelist_print(print_object);
  printf("BREAK\n");

  arr[1] = (int *) my_malloc(10 * sizeof(int));

  freelist_print(print_object);
  printf("BREAK\n");
  return 0;
} /* main() */
