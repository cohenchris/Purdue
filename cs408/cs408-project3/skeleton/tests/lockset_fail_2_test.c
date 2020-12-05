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

void *foo(void *arg) {
  pthread_mutex_lock(&lock2);
  pthread_mutex_lock(&lock);
  printf("WRITING1\n");
  DC_WRITE(i);
  pthread_mutex_unlock(&lock2);
  pthread_mutex_unlock(&lock);
  return (void*)0;
}

void* foo2(void * arg) {
  pthread_mutex_lock(&lock2);
  pthread_mutex_lock(&lock3);
  printf("WRITING2\n");
  DC_WRITE(i);
  pthread_mutex_unlock(&lock2);
  pthread_mutex_unlock(&lock3);

  return (void*)0;
}

void* foo3(void * arg) {
  pthread_mutex_lock(&lock);
  pthread_mutex_lock(&lock3);
  printf("WRITING3\n");
  DC_WRITE(i);
  pthread_mutex_unlock(&lock);
  pthread_mutex_unlock(&lock3);

  return (void*)0;
}

int main() {
  pthread_t thread;
  pthread_t thread2;
  pthread_t thread3;
  printf("LOCK 1 = %p\n", &lock);
  printf("LOCK 2 = %p\n", &lock2);
  printf("LOCK 3 = %p\n", &lock3);

  pthread_create(&thread, NULL, &foo, NULL);
  sleep(1);
  pthread_create(&thread2, NULL, &foo2, NULL);
  sleep(1);
  pthread_create(&thread3, NULL, &foo3, NULL);
  pthread_exit(NULL);
  return 0;
}
