#include <stdio.h>
#include <unistd.h>
#include <assert.h>

#include "test_funcs.h"
#include "my_malloc.h"
#include "printing.h"

/*
 * Ensure chunk coalescing is done properly
 */

int main()
{
  int * arr[3];

  //  Allocate chunk 1
  arr[0] = (int *) my_malloc(ARENA_SIZE - 16 * ALLOC_HEADER_SIZE);
  assert(arr[0] != NULL);
    
  freelist_print(print_object);
  printf("BREAK\n");
  
  //  Allocate chunk 2 (should coalesce)
  arr[1] = (int *) my_malloc(ARENA_SIZE - 16 * ALLOC_HEADER_SIZE);
  assert(arr[1] != NULL);

  freelist_print(print_object);
  printf("BREAK\n");
  verify_header_count(1, 2, 2);

  //  Break address space contiguity
  sbrk(100);

  arr[2] = (int *) my_malloc(ARENA_SIZE - 16 * ALLOC_HEADER_SIZE);
  assert(arr[2] != NULL);

  header * h = (header *) g_base;

  //  The freelist should now have 2 entries
  assert(h->next != NULL);

  freelist_print(print_object);

  return 0;
} /* main() */
