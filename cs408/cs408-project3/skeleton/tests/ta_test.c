#include<stdio.h>
#include<pthread.h>
#include<unistd.h>
#include"../libaccess.h"
int i;

/*
 * LOCKSET        - Should FAIL
 * HAPPENS-BEFORE - Should FAIL
 */

pthread_mutex_t lock;
void *foo(void *arg) {
    pthread_mutex_lock(&lock);
    DC_READ(i);
    pthread_mutex_unlock(&lock);
    DC_READ(i);
    return (void *)0;
}

void *bar(void *arg) {
    pthread_mutex_lock(&lock);
    DC_WRITE(i);
    pthread_mutex_unlock(&lock);
    return (void *)0;    
}

int main() {
    pthread_t thread;
    pthread_t threadBar;
    pthread_create(&thread, NULL, &foo, NULL);
    pthread_create(&threadBar, NULL, &bar, NULL);

    pthread_exit(NULL);
    return 0;
}