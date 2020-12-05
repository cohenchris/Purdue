#include <stdio.h>

#include "my_malloc.h"
#include "printing.h"

/*
 * Ensure data is written to the correct spot
 */

int main()
{
  int * arr[2];

  arr[0] = (int *) my_malloc(16 * sizeof(int));

  for (int i = 0; i < 16; i++) {
    arr[0][i] = i + 0xFFFFFF00;
  }

  arr[1] = (int *) my_malloc(16 * sizeof(int));

  for (int i = 0; i < 16; i++) {
    arr[1][i] = i + 0xFFFFFF00;
  }

  for (size_t * p = g_base;
    (char *) p < ((char *) g_base) + 3 * ALLOC_HEADER_SIZE + 32 * sizeof(int);
    p++)
  {
    printf("%p\n", (void *)*p);
  }

  my_free(arr[0]);
  my_free(arr[1]);

  for (size_t * p = g_base;
    (char *) p < ((char *) g_base) + 3 * ALLOC_HEADER_SIZE + 32 * sizeof(int);
    p++) 
  {
    printf("%p\n", (void *)*p);
  }

  return 0;
} /* main() */
