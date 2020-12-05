#include<stdio.h>
#include<string.h>
#include<pthread.h>
#include<unistd.h>
#include<malloc.h>

pthread_mutex_t lock1 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t lock2 = PTHREAD_MUTEX_INITIALIZER;

/*
 * PTHREAD_MUTEX_TRYLOCK TEST
 * This test uses trylock in order to prevent a deadlock. This should prevent deadlock.
 * If it does deadlock, it will be caught by the framework and return 1.
 */

void *t1(void * args) {
  // lock mutex 1
  pthread_mutex_lock(&lock1);
  // lock mutex 2
  pthread_mutex_lock(&lock2);

  // unlock mutex 2
  pthread_mutex_unlock(&lock2);
  // unlock mutex 1
  pthread_mutex_unlock(&lock1);
  pthread_exit(NULL);
}

void *t2(void * args) {
  for (; ;) {
    // lock mutex 2 when thread 1 releases it
    pthread_mutex_lock(&lock2);

    // try to aquire mutex 1
    if (pthread_mutex_trylock(&lock1) == 0) {
        printf("Aquired lock 1\n");
        fflush(stdout);
        break;
    // unlock mutex 2 to avoid deadlock
    } else {
        printf("Unlock lock 2\n");
        pthread_mutex_unlock(&lock2);
        fflush(stdout);
    }
  }
  pthread_mutex_unlock(&lock1);
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

  // If it reaches here, it succeeds. If it deadlocks, the framework will catch it and return 1.
  return 0;
}