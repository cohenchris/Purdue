#include<stdio.h>
#include<string.h>
#include<pthread.h>
#include<unistd.h>
#include<malloc.h>

pthread_mutex_t lock1 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t lock2 = PTHREAD_MUTEX_INITIALIZER;

/*
 * DEADLOCK TEST
 * This test will deadlock most of the time. A program should never deadlock.
 * If it does deadlock, it will be caught by the framework, returning 1.
 */

void *t1(void * args) {
  // lock mutex 1
  pthread_mutex_lock(&lock1);

  for (int i = 0; i < 100000; i++)
    ;

  // lock mutex 2
  pthread_mutex_lock(&lock2);
  pthread_mutex_unlock(&lock1);
  pthread_exit(NULL);
}

void *t2(void * args) {
  pthread_mutex_lock(&lock2);

  for (int i = 0; i < 100000; i++)
    ;
  
  pthread_mutex_lock(&lock1);
  pthread_mutex_unlock(&lock2);
  pthread_exit(NULL);
}

int main() {
  pthread_t thread1;
  pthread_t thread2;

  pthread_create(&thread1, NULL, &t1, NULL);
  pthread_create(&thread2, NULL, &t2, NULL);

  pthread_join(thread1, NULL);
  pthread_join(thread2, NULL);

  // If it somehow reaches here, it hasn't deadlocked, and the program was a success
  return 0;
}