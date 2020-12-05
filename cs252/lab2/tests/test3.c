#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <assert.h>
#include "my_malloc.h"

void *__real_sbrk(intptr_t increment);

/*
 * Simulate a failed sbrk
 */

void *__wrap_sbrk(intptr_t increment)
{
  return (void *) -1;
} /* __wrap_sbrk() */

/*
 * Test behavior on sbrk fail
 */

int main()
{
  int * arr = (int *) my_malloc(16 * sizeof(int));

  assert(arr == NULL);
  assert(errno == ENOMEM);

  return 0;
} /* main() */
