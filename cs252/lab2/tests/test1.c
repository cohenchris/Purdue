#include <stdio.h>

#include "test_funcs.h"
#include "my_malloc.h"

#define NUM_INTS (4)

/*
 * Test that the correct number of alloc, free, and fencepost blocks
 * are created for a simple job (malloc only)
 *
 * Ensure malloc(0) returns NULL
 */

int main()
{
  int *arr[NUM_INTS];

  arr[0] = (int *) my_malloc(0);
  assert(arr[0] == NULL);

  for (int i = 0; i < NUM_INTS; i++) {
    arr[i] = (int *) my_malloc(16 * sizeof(int));
    verify_header_count(1, i + 1, 2);
  }

  return 0;
} /* main() */
