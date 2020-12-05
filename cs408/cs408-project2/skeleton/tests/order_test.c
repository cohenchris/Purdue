#include<stdio.h>
#include<string.h>
#include<pthread.h>
#include<unistd.h>
#include<malloc.h>

/*
 * ORDER TEST
 * This test runs a program where 1 thread updates a name string from "Joe" to "Chris, and the other prints the name variable.
 * The order of pthread_create and pthread_join calls ensures that the string is always updated to "Chris" before it is printed.
 * Therefore, the printed string can only ever be "Your name IS 'Chris'"
 */

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
char *name = NULL;
int return_code;

void *t1(void * args) {
    name = "Chris";
  pthread_exit(NULL);
}

void *t2(void * args) {
    if (strcmp("Chris", name) == 0) {
        return_code = 0;
        printf("Your name IS 'Chris'\n");
    }
    else {
        return_code = 1;
        printf("Your name is NOT 'Chris'\n");
    }
    pthread_exit(NULL);
}

int main() {
  name = "Joe";
  pthread_t thread1;
  pthread_t thread2;

  pthread_create(&thread1, NULL, &t1, NULL);
  pthread_join(thread1, NULL);
  pthread_create(&thread2, NULL, &t2, NULL);
  pthread_join(thread2, NULL);

  return return_code;
}