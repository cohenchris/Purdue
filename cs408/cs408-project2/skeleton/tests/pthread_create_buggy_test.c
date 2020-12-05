#include<stdio.h>
#include<pthread.h>
#include<unistd.h>

#define THREAD_NUM 10

/*
 * PTHREAD_CREATE BUGGY TEST
 * This test creates 10 threads and re-uses the 'args' struct, causing possible data races.
 * The threads should print out i*i for every i in range 0-9, but sometimes, two will print out the same thing because of data races.
 */

struct multipliers {
    int x;
    int y;
};

void* multiply(void * args) {
  struct multipliers *arguments = (struct multipliers *)args;
  int result = (arguments->x) * (arguments->y);
  printf("%d * %d = %d\n", arguments->x, arguments->y, result);
  fflush(stdout);
  return (void *)result;
}

int main() {
  pthread_t threads[THREAD_NUM];

  for (int i = 0; i < THREAD_NUM; i++) {
    struct multipliers args;
    args.x = i;
    args.y = i;

    pthread_create(&threads[i], NULL, &multiply, (void *)&args);
  }

  for (int i = 0; i < THREAD_NUM; i++) {
    int return_val;
    int expected = i*i;
    pthread_join(threads[i], (void **)&return_val);
    if ((int)return_val != expected) {
      printf("Expected %d, got %d\n", expected, (int)return_val);
      return 1;
    }
  }
  return 0;
}
