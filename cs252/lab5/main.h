#ifndef MAIN_H
#define MAIN_H

typedef enum {
  E_NO_CONCURRENCY = 0,
  E_FORK_PER_REQUEST = 'f',
  E_THREAD_PER_REQUEST = 't',
  E_THREAD_POOL = 'p'
} concurrency_mode;

#endif // MAIN_H
