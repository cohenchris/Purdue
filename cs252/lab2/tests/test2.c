#include <assert.h>
#include <stdio.h>
#include <unistd.h>

#include "my_malloc.h"

/*
 * Ensures that no memory is allocated before the program begins
 *
 * Checks for valid sbrk size allocations (multiples of ARENA_SIZE)
 */

int main()
{
  assert(sbrk(0) == g_base);

  int * arr = (int *) my_malloc(ARENA_SIZE - 2 * ALLOC_HEADER_SIZE);
  assert(arr != NULL);
  assert(((char *)sbrk(0)) == ((char *)g_base) + 2 * ARENA_SIZE);

  assert(g_freelist_head->next == NULL);
  assert(g_freelist_head->size == ARENA_SIZE - 2 * ALLOC_HEADER_SIZE);

  return 0;
} /* main() */
