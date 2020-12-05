#include <stdio.h>
#include <assert.h>

#include "my_malloc.h"
#include "printing.h"

#define NUM_INTS (4)

/*
 * Test that the correct fencepost blocks
 * are created for a simple job (malloc & free)
 * (the diff version of 1)
 */

int main()
{
  int * arr[NUM_INTS];

  for (int i = 0; i < NUM_INTS; i++) {
    arr[i] = (int *) my_malloc(16 * sizeof(int));
    assert(arr[i] != NULL);
    freelist_print(print_object);
  }

  return 0;
} /* main() */
