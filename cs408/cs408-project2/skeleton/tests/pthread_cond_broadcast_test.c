#include<stdio.h>
#include<string.h>
#include<pthread.h>
#include<unistd.h>
#include<malloc.h>

#define THREAD_NUM 10

/*
 * PTHREAD_COND_BROADCAST TEST
 * This test has 10 threads wait on 1 condition variable, and have another thread broadcast a signal for that condition variable
 * to make all of the threads start executing again.
 */

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
pthread_cond_t wait_cond = PTHREAD_COND_INITIALIZER;

int g_waiting_count = 0;

struct my_thread {
    int id;
};

void *t1(void * args) {
  struct my_thread *arguments = (struct my_thread *)args;

  pthread_mutex_lock(&lock);
  printf("Thread %d waiting for condition\n", arguments->id);
  fflush(stdout);

  g_waiting_count +=1;
  if (g_waiting_count == THREAD_NUM) {
    pthread_cond_signal(&wait_cond);
  }
  pthread_cond_wait(&cond, &lock);
  printf("Thread %d terminating\n", arguments->id);
  pthread_mutex_unlock(&lock);
  pthread_exit(NULL);
}

void *t2(void * args) {
  pthread_mutex_lock(&lock);
  while (g_waiting_count < THREAD_NUM) {
    pthread_cond_wait(&wait_cond, &lock);
  }
  printf("\nBROADCASTING CONDITION SIGNAL\n\n");
  pthread_cond_broadcast(&cond);
  pthread_mutex_unlock(&lock);
  pthread_exit(NULL);
}

int main() {
  pthread_t threads[THREAD_NUM];
  struct my_thread args[THREAD_NUM];

  for (int i = 0; i < THREAD_NUM; i++) {
    args[i].id = i + 1;
    pthread_create(&threads[i], NULL, &t1, (void *)&args[i]);
  }

  pthread_t dispatcher;
  pthread_create(&dispatcher, NULL, &t2, NULL);

  for (int i = 0; i < THREAD_NUM; i++) {
    pthread_join(threads[i], NULL);
  }
  pthread_join(dispatcher, NULL);

  // If the program gets down to here, it succeeds. The program will hang if it fails.
  return 0;
}