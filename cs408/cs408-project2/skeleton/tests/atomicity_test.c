#include<stdio.h>
#include<string.h>
#include<pthread.h>
#include<unistd.h>
#include<malloc.h>

/*
 * ATOMICITY TEST
 * This test runs a program where 1 thread verifies that a string is "Chris", and the other one frees it.
 * The order of pthread_create and pthread_join calls ensures that the string is always verified before it is freed.
 * Therefore, the verification can only ever print "The string IS 'Chris'"
 */

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
char *name = NULL;
int return_code;

void *t1(void * args) {
    if (strcmp("Chris", name) == 0) {
        return_code = 0;
        printf("The string IS 'Chris'\n");
    }
    else {
        return_code = 1;
        printf("The string is NOT 'Chris'\n");
    }
    pthread_exit(NULL);
}

void *t2(void * args) {
    free(name);
    name = NULL;
    pthread_exit(NULL);
}

int main() {
    name = malloc(sizeof(char) * 6);
    strcpy(name, "Chris");

    pthread_t thread1;
    pthread_t thread2;

    pthread_create(&thread1, NULL, &t1, NULL);
    pthread_join(thread1, NULL);
    pthread_create(&thread2, NULL, &t2, NULL);
    pthread_join(thread2, NULL);

    return return_code;
}