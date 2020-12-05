#ifndef TESTLIB_H
#define TESTLIB_H

#include <stdlib.h>

/*
#ifndef MEMORY_ACCESS
#define MEMORY_ACCESS

void dc_write_handle(void *, int, const char *);
void dc_read_handle(void *, int, const char *);

#define DC_WRITE(X) dc_write_handle(&X, sizeof(X), #X)
#define DC_READ(X) dc_read_handle(&X, sizeof(X), #X)

#endif
*/
/*
 * The following functions must be implemented.
 * Calls to these library functions will be intercepted with ld_preload
 * This header file should not get changed.
 */

// Thread Management
int pthread_create(pthread_t *thread, const pthread_attr_t *attr, void *(*start_routine) (void *), void *arg);
void pthread_exit(void *retval);
int pthread_yield(void);

// Mutexes
int pthread_mutex_lock(pthread_mutex_t *mutex);
int pthread_mutex_unlock(pthread_mutex_t *mutex);
int pthread_mutex_trylock(pthread_mutex_t *mutex);

#endif
