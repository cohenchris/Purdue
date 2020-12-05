#ifndef TESTLIB_H
#define TESTLIB_H

#include <stdlib.h>

/*
 * The following functions must be implemented.
 * Calls to these library functions will be intercepted with ld_preload
 * This header file should not get changed.
 */

// Thread Management
int pthread_create(pthread_t *thread, const pthread_attr_t *attr, void *(*start_routine) (void *), void *arg);
void pthread_exit(void *retval);
int pthread_yield(void);

// Condition variables
int pthread_cond_wait(pthread_cond_t *cond, pthread_mutex_t *mutex);
int pthread_cond_signal(pthread_cond_t *cond);
int pthread_cond_broadcast(pthread_cond_t *cond);

// Mutexes
int pthread_mutex_lock(pthread_mutex_t *mutex);
int pthread_mutex_unlock(pthread_mutex_t *mutex);
int pthread_mutex_trylock(pthread_mutex_t *mutex);

#endif
