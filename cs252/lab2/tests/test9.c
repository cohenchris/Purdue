#include <assert.h>
#include <stdio.h>

#include "my_malloc.h"
#include "printing.h"

/*
 * Ensure space is allocated in the correct sizes
 */

int main()
{
  char * arr = (char *) my_malloc(1);

  size_t size = ((header *)(arr - ALLOC_HEADER_SIZE))->size;

  assert((size - 1) % MIN_ALLOCATION == 0);
  assert((size - 1) >= 2 * sizeof(header *));
  assert((size - 1) <= 2 * sizeof(header *) + MIN_ALLOCATION);

  arr = (char *) my_malloc(17);

  size = ((header *)(arr - ALLOC_HEADER_SIZE))->size;

  assert((size - 1) % MIN_ALLOCATION == 0);
  assert((size - 1) >= 2 * sizeof(header *));

  return 0;
} /* main() */
