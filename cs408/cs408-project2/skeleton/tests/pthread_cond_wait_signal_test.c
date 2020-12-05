#include<stdio.h>
#include<string.h>
#include<pthread.h>
#include<unistd.h>
#include<malloc.h>

/*
 * PTHREAD_COND_WAIT AND PTHREAD_COND_SIGNAL TEST
 * This test has one thread wait on a condition variable and the other thread signals that condition variable, resuming the first thread's execution.
 */

pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

void *t1(void * args) {
  pthread_mutex_lock(&lock);

  printf("Waiting for condition\n");
  pthread_cond_wait(&cond, &lock);

  pthread_mutex_unlock(&lock);
  pthread_exit(NULL);
}

void *t2(void * args) {
  pthread_mutex_lock(&lock);

  printf("Signaling condition\n");
  pthread_cond_signal(&cond);

  pthread_mutex_unlock(&lock);
  pthread_exit(NULL);
}

int main() {
  pthread_t thread1;
  pthread_t thread2;

  pthread_create(&thread1, NULL, &t1, NULL);
  sleep(0.1);
  pthread_create(&thread2, NULL, &t2, NULL);

  pthread_join(thread1, NULL);
  pthread_join(thread2, NULL);

  // If the program executes to here, it succeeds.
  return 0;
}