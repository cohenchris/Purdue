#include <stdio.h>

#include "my_malloc.h"
#include "printing.h"

/*
 * Free NULL does nothing
 *
 * Double free throws an error
 */

int main()
{
  int * arr = (int *) my_malloc(16 * sizeof(int));

  freelist_print(print_object);

  my_free(NULL);
  my_free(arr);
  freelist_print(print_object);

  my_free(arr);

  freelist_print(print_object);

  return 0;
} /* main() */
