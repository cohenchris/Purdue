#include<stdio.h>
#include<pthread.h>
#include<unistd.h>
#include"../libaccess.h"

/*
 * LOCKSET        - Should FAIL
 * HAPPENS-BEFORE - Should FAIL
 */

int i = 0;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t lock2 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t lock3 = PTHREAD_MUTEX_INITIALIZER;

void *foo2(void *arg) {
  pthread_mutex_lock(&lock2);
  printf("READING\n");
  DC_READ(i);
  pthread_mutex_unlock(&lock2);
  return (void*)0;
}

void* foo(void * arg) {
  pthread_mutex_lock(&lock);
  printf("WRITING\n");
  DC_WRITE(i);
  pthread_mutex_unlock(&lock);

  return (void*)0;
}

int main() {
  printf("LOCK 1 = %p\n", &lock);
  printf("LOCK 2 = %p\n", &lock2);

  pthread_t thread;
  pthread_t thread2;
  pthread_create(&thread, NULL, &foo, NULL);
  pthread_create(&thread2, NULL, &foo2, NULL);
  pthread_exit(NULL);
  return 0;
}
