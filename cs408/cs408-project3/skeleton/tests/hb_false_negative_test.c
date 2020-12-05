#include<stdio.h>
#include<pthread.h>
#include<unistd.h>
#include"../libaccess.h"

/*
 * LOCKSET        - Should FAIL
 * HAPPENS-BEFORE - Should PASS (but is false negative)
 */

int i = 0;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t lock2 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t lock3 = PTHREAD_MUTEX_INITIALIZER;


void* foo(void * arg) {
  printf("WRITING\n");
  DC_WRITE(i);

  pthread_mutex_lock(&lock);
  pthread_mutex_unlock(&lock);

  return (void*)0;
}

void *foo2(void *arg) {
  sleep(1);

  pthread_mutex_lock(&lock);
  pthread_mutex_unlock(&lock);

  printf("READING\n");
  DC_READ(i);

  return (void*)0;
}

int main() {
  pthread_t thread;
  pthread_t thread2;
  pthread_create(&thread, NULL, &foo, NULL);
  pthread_create(&thread2, NULL, &foo2, NULL);
  pthread_exit(NULL);
  return 0;
}
