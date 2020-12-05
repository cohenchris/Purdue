#include<stdio.h>
#include<pthread.h>
#include<unistd.h>
#include"../libaccess.h"

/*
 * LOCKSET        - Should PASS
 * HAPPENS-BEFORE - Should PASS
 */

int i = 0;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t lock2 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t lock3 = PTHREAD_MUTEX_INITIALIZER;

void *foo2(void *arg) {
  pthread_mutex_lock(&lock);
  printf("READING\n");
  DC_READ(i);
  pthread_mutex_unlock(&lock);
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
  pthread_t thread;
  pthread_t thread2;
  // DC_READ(i);
  // DC_WRITE(i);
  pthread_create(&thread, NULL, &foo, NULL);
  pthread_create(&thread2, NULL, &foo2, NULL);
  pthread_exit(NULL);
  return 0;
}
