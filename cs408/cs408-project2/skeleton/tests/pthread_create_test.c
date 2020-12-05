#include<stdio.h>
#include<pthread.h>
#include<unistd.h>
#include<malloc.h>

#define THREAD_NUM 10

/*
 * PTHREAD_CREATE TEST
 * This test creates 10 threads and makes sure to pass a unique 'args' struct, preventing data races.
 * The threads will print out i*i for every i in range 0-9.
 */

struct multipliers {
    int x;
    int y;
};

void *multiply(void * args) {
  struct multipliers *arguments = (struct multipliers *)args;
  int result = (arguments->x) * (arguments->y);
  printf("%d * %d = %d\n", arguments->x, arguments->y, result);
  fflush(stdout);
  return result;
}

int main() {
  pthread_t threads[THREAD_NUM];
  struct multipliers args[THREAD_NUM];

  for (int i = 0; i < THREAD_NUM; i++) {
    args[i].x = i;
    args[i].y = i;

    pthread_create(&threads[i], NULL, &multiply, (void *)&args[i]);
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
