#define _GNU_SOURCE
#include <assert.h> // Use this, it is your friend!
#include <libunwind.h>
#include <stdlib.h>

#include <unistd.h>
#include <sys/syscall.h>
#define gettid() syscall(SYS_gettid)

#include <semaphore.h>
#include <string.h>

#define UNW_LOCAL_ONLY
#include <libunwind.h>
#include <stdio.h>

#include <stdbool.h>

#include "testlib.h"
#include "utils.h"

#include <dlfcn.h>
typedef void (*start_routine_type)();
typedef int (*pthread_create_type)();
typedef void (*pthread_exit_type)();
typedef int (*pthread_yield_type)();
typedef int (*pthread_cond_wait_type)();
typedef int (*pthread_cond_signal_type)();
typedef int (*pthread_cond_broadcast_type)();
typedef int (*pthread_mutex_lock_type)();
typedef int (*pthread_mutex_unlock_type)();
typedef int (*pthread_mutex_trylock_type)();

// Needed for PCT
#define DEBUG true
#define MAX_THREADS 64
// This may be overkill but any thread can have any number of locks - incomplete
#define MAX_MUTEXES 100

sem_t g_count_lock;
sem_t g_print_lock;

// Total number of threads that are active
int g_thread_count = 0;
int STACKTRACE_THREAD_ID = -1;

// Keeps track of thread counts - index is gettid()
// We were told that there will only be up to 64 threads ever run
long int g_thread_ids[MAX_THREADS] = { 0 };


// Used for interpose_start_routine in order to pass multiple args
typedef struct arg_struct {
  void *(*struct_func) (void *);
  void *struct_arg;
  // Needed for PCT
  int thread_index;
} arg_struct;

// PCT thread state

#define PCT_INIT_MAIN 0
#define PCT_THREAD_CALL 1
#define PCT_THREAD_BEFORE_CREATE 2
#define PCT_THREAD_AFTER_CREATE 3
#define PCT_THREAD_START 4
#define PCT_THREAD_TERMINATE 5
#define PCT_THREAD_YIELD 6
#define PCT_THREAD_LOCK 7
#define PCT_THREAD_UNLOCK 8
#define PCT_DO_NOTHING 9

// A thread can have any of the following states
// - does not currently exist (never created or terminated)
// - running
// - block
#define THREAD_DEAD 0
#define THREAD_RUNNABLE 1
#define THREAD_BLOCKED 2

struct thread_struct {
  // Proccess ID for current thread
  pthread_t thread_id;
  int state;
};

// g_current_thread is the index of the current thread
// Used this in PCT_thread_start() to know that
// PCT_thread_create() was done
int g_current_thread = -1;

int g_runnable_threads = 0;
int g_block_threads = 0;

// Make array of threads with boolean indicating if they are runnable or not
// (Simpler data structures and a linked list)
struct thread_struct *g_threads = NULL;

// Array of semaphores for each thread mapped to the g_runnable array
sem_t *g_semaphores = NULL;

// Structure for mutexes this in incomplete 
struct mutex_struct{
  pthread_mutex_t *mutex;
  int num_threads_waiting;
  int threads_waiting[MAX_THREADS];
};

// Array of currently active mutexes (incomplete)
struct mutex_struct *g_thread_mutexes = NULL;

// Mutex lock used in the PCT algorithm
pthread_mutex_lock_type g_orig_mutex_lock;
pthread_mutex_unlock_type g_orig_mutex_unlock;

////////////////////////////////////////////////////
///////////////////// HELPERS //////////////////////
////////////////////////////////////////////////////

// Used to find the thread's thread number (assigned via g_thread_count)
int find_thread_number(long int tid) {
  int thread_ids_size = sizeof(g_thread_ids) / sizeof(g_thread_ids[0]);
  for (int i = 0; i < thread_ids_size; i++) {
    if (g_thread_ids[i] == tid) {
      return i;
    }
  }
  return -1;
}

////////////////////////////////////////////////////
//////////////////// STACKTRACE ////////////////////
////////////////////////////////////////////////////

// String array of functions to omit from stack trace
char omit_functions[4][25] = {
  "interpose_start_routine",
  "omit",
  "stacktrace",
  "find_thread_number"
};

bool omit(char * func) {
  int arr_size = sizeof(omit_functions) / sizeof(omit_functions)[0];
  for (int i = 0; i < arr_size; i++) {
    if (strcmp((char *)func, (char *)omit_functions[i]) == 0) {
      return true;
    }
  }
  return false;
}

void stacktrace() {
  if (get_stacktraces()) {
    unw_cursor_t cursor;
    unw_context_t context;
    
    // Initialize cursor to current frame for local unwinding.
    unw_getcontext(&context); // Takes a snapshot of the current CPU registers
    unw_init_local(&cursor, &context);  // Initializes the cursor to beginning of 'context' (HANGING RN)

    INFO("Stacktrace: \n");
    fflush(stdout);
    // Unwind frames one by one, going up the frame stack. 
    while (unw_step(&cursor) > 0) {
        unw_word_t offset, pc; 
        unw_get_reg(&cursor, UNW_REG_IP, &pc);
        if (pc == 0) {
            break; 
        }
        char sym[256];
        if (unw_get_proc_name(&cursor, sym, sizeof(sym), &offset) == 0) {
          if (!omit(sym)) {
            // Makes sure that ONLY stack trace for target program exists.
            INFO("  0x%lx: (%s+0x%lx)\n", pc, sym, offset);
            fflush(stdout);
          }
        } else {
            INFO("  0x%lx: -- ERROR: unable to obtain symbol name for this frame\n", pc); 
            fflush(stdout);
        }
    }
  }
  STACKTRACE_THREAD_ID = -1;
}

////////////////////////////////////////////////////
//////////////////// PCT ///////////////////////////
////////////////////////////////////////////////////

int find_highest_priority() {
  sem_t PCT_lock;
  sem_init(&PCT_lock, 0, 1);
  sem_wait(&PCT_lock);

  // Find the highest priority thread available to be active
  int highest_priorty = -1;
  int thread_index = -1;

  for (int i = 0; i < MAX_THREADS; i++) {
    if ((g_threads[i].state == THREAD_RUNNABLE) && (get_priorities()[i] > highest_priorty)) {
      thread_index = i;
      highest_priorty = get_priorities()[thread_index];
    } 
  }

  sem_post(&PCT_lock);
  return thread_index;
}

void run_highest_priority() {
  sem_t PCT_lock;
  sem_init(&PCT_lock, 0, 1);
  sem_wait(&PCT_lock);

  if (DEBUG) {
    sem_wait(&g_print_lock);
    INFO("ENTER run_highest_priority() - g_runnable_threads = %d - g_current_thread = %d \n", 
         g_runnable_threads, g_current_thread);
    fflush(stdout);
    sem_post(&g_print_lock);
  }

  // Mark the g_current_thread as stopped
  sem_init(&(g_semaphores[g_current_thread]), 0, 0);
  // Unblock the highest priority thread
  g_current_thread = find_highest_priority();

  if (DEBUG) {
    sem_wait(&g_print_lock);
    INFO("EXIT run_highest_priority() - g_runnable_threads = %d - g_current_thread = %d \n", 
         g_runnable_threads, g_current_thread);
    fflush(stdout);
    sem_post(&g_print_lock);
  }

  if (g_runnable_threads > 0) {
    if (DEBUG) {
      sem_wait(&g_print_lock);
      INFO("POSTING THREAD: %d", g_current_thread);
      fflush(stdout);
      sem_post(&g_print_lock);
    }
    // Let PCT_thread_start() know that this thread can run
    sem_post(&(g_semaphores[g_current_thread]));
  }
  sem_post(&PCT_lock);
  return;
}

int find_next_available_thread() {
  sem_t PCT_lock;
  sem_init(&PCT_lock, 0, 1);
  sem_wait(&PCT_lock);

  // Find the highest priority thread available to be active
  int highest_priorty = -1;
  int thread_index = -1;

  for (int i = 0; i < MAX_THREADS; i++) {
    if ((g_threads[i].state == THREAD_DEAD) && (get_priorities()[i] > highest_priorty)) {
      thread_index = i;
      highest_priorty = get_priorities()[i];
    } 
  }

  sem_post(&PCT_lock);
  return thread_index;
}

void PCT_init_main() {
  sem_t PCT_lock;
  sem_init(&PCT_lock, 0, 1);
  sem_wait(&PCT_lock);

  if (DEBUG) {
    sem_wait(&g_print_lock);
    INFO("ENTER PCT_init_main() - g_runnable_threads = %d - g_current_thread = %d \n", 
         g_runnable_threads, g_current_thread);
    fflush(stdout);
    sem_post(&g_print_lock);
  }

  // Randomly choose priority for the main thread per a piazza post -
  // Therefore choosing thread index 0
  int thread_index = 0;
  // This is the thread_id for the main thread
  g_threads[thread_index].thread_id = gettid();
  g_threads[thread_index].state = THREAD_RUNNABLE;
  g_current_thread = thread_index;

  if (DEBUG) {
    sem_wait(&g_print_lock);
    INFO("POSTING thread: %d \n", g_current_thread);
    fflush(stdout);
    sem_post(&g_print_lock);
  }

  sem_post(&(g_semaphores[g_current_thread]));
  // thread count = 0 per section 4.2 top of page 6
  g_runnable_threads++;

  if (DEBUG) {
    sem_wait(&g_print_lock);
    INFO("EXITING PCT_init_main() - g_runnable_threads = %d - g_current_thread = %d - priority = %d - proccess id = %ld \n", 
         g_runnable_threads, g_current_thread, get_priorities()[g_current_thread], g_threads[g_current_thread].thread_id);
    fflush(stdout);
    sem_post(&g_print_lock);
  }

  sem_post(&PCT_lock);
  return;
}

void PCT_thread_start() {
  sem_t PCT_lock;
  sem_init(&PCT_lock, 0, 1);
  sem_wait(&PCT_lock);

  if (DEBUG) {
    sem_wait(&g_print_lock);
    INFO("ENTER PCT_thread_start() - g_runnable_threads = %d - g_current_thread = %d \n", 
         g_runnable_threads, g_current_thread);
    fflush(stdout);
    sem_post(&g_print_lock);
  }

  // Store the thread_id for the current thread for later use maybe in mutexes
  g_threads[g_current_thread].thread_id = gettid();
  // Wait until you are posted
  sem_wait(&(g_semaphores[g_current_thread]));

  if (DEBUG) {
    sem_wait(&g_print_lock);
    INFO("EXITING PCT_thread_start() - g_runnable_threads = %d - new_thread = %d - priority = %d - proccess id = %ld \n", 
         g_runnable_threads, g_current_thread, get_priorities()[g_current_thread], g_threads[g_current_thread].thread_id);
    fflush(stdout);
    sem_post(&g_print_lock);
  }

  sem_post(&PCT_lock);
  return;
}

void PCT_thread_after_create() {
  sem_t PCT_lock;
  sem_init(&PCT_lock, 0, 1);
  sem_wait(&PCT_lock);

  if (DEBUG) {
    sem_wait(&g_print_lock);
    INFO("ENTER PCT_thread_after_create() - g_runnable_threads = %d - g_current_thread = %d \n", 
         g_runnable_threads, g_current_thread);
    fflush(stdout);
    INFO("POSTING thread: %d\n", g_current_thread);
    fflush(stdout);
    sem_post(&g_print_lock);
  }

  // Unblock highest priority thread
  g_current_thread = find_highest_priority();
  sem_post(&(g_semaphores[g_current_thread]));
  
  if (DEBUG) {
    sem_wait(&g_print_lock);
    INFO("EXITING PCT_thread_after_create() - g_runnable_threads = %d - new_thread = %d - priority = %d \n", 
         g_runnable_threads, g_current_thread, get_priorities()[g_current_thread]);
    fflush(stdout);
    sem_post(&g_print_lock);
  }

  sem_post(&PCT_lock);
  return;
}

void PCT_thread_before_create() {
  sem_t PCT_lock;
  sem_init(&PCT_lock, 0, 1);
  sem_wait(&PCT_lock);

  if (DEBUG) {
    sem_wait(&g_print_lock);
    INFO("ENTER PCT_thread_before_create() - g_runnable_threads = %d - g_current_thread = %d \n", 
         g_runnable_threads, g_current_thread);
    fflush(stdout);
    sem_post(&g_print_lock);
  }

  // Mark thread is runnable
  // Find the priority for this thread - from highest priority where thread is not active yet (ie DEAD)
  g_current_thread = find_next_available_thread();
  g_threads[g_current_thread].state = THREAD_RUNNABLE;
  // the thread_id cannot be assigned until PCT_thread_start() when the thread has actually start
  // PCT_thread_start is called indirectly from interpose start routine through run_algorithm()
  g_runnable_threads++;

  if (DEBUG) {
    sem_wait(&g_print_lock);
    INFO("EXITING PCT_thread_before_create() - g_runnable_threads = %d - g_current_thread = %d - priority = %d \n", 
         g_runnable_threads, g_current_thread, get_priorities()[g_current_thread]);
    fflush(stdout);
    sem_post(&g_print_lock);
  }

  sem_post(&PCT_lock);
  return;
}

void PCT_thread_terminate() {
  sem_t PCT_lock;
  sem_init(&PCT_lock, 0, 1);
  sem_wait(&PCT_lock);

  if (DEBUG) {
    sem_wait(&g_print_lock);
    INFO("ENTER PCT_thread_terminate() - g_runnable_threads = %d - g_current_thread = %d \n", 
         g_runnable_threads, g_current_thread);
    fflush(stdout);
    sem_post(&g_print_lock);
  }

  // pthread_exit - or termination of thread
  // set g_current thread to be the next runnable thread
  g_threads[g_current_thread].state = THREAD_DEAD;
  g_runnable_threads--;

  run_highest_priority();

  if (DEBUG) {
    sem_wait(&g_print_lock);
    INFO("EXITING PCT_thread_terminate() - g_runnable_threads = %d - g_current_thread = %d \n", 
         g_runnable_threads, g_current_thread);
    fflush(stdout);
    sem_post(&g_print_lock);
  }

  sem_post(&PCT_lock);
  return;
}

void PCT_thread_yield() {
  sem_t PCT_lock;
  sem_init(&PCT_lock, 0, 1);
  sem_wait(&PCT_lock);

  if (DEBUG) {
    sem_wait(&g_print_lock);
    INFO("ENTER PCT_thread_yield() - g_runnable_threads = %d - g_current_thread = %d \n", 
         g_runnable_threads, g_current_thread);
    fflush(stdout);
    sem_post(&g_print_lock);
  }

  // incomplete
  run_highest_priority();

  if (DEBUG) {
    sem_wait(&g_print_lock);
    INFO("g_current_thread = %d \n", g_current_thread);
    fflush(stdout);
    sem_post(&g_print_lock);
  }

  if (DEBUG) {
    sem_wait(&g_print_lock);
    INFO("EXITING PCT_thread_yield() - g_runnable_threads = %d - g_current_thread = %d \n", 
         g_runnable_threads, g_current_thread);
    fflush(stdout);
    sem_post(&g_print_lock);
  }

  sem_post(&PCT_lock);
  return;
}

void PCT_thread_lock() {
  sem_t PCT_lock;
  sem_init(&PCT_lock, 0, 1);
  sem_wait(&PCT_lock);

  if (DEBUG) {
    sem_wait(&g_print_lock);
    INFO("ENTER PCT_thread_lock() - g_runnable_threads = %d - g_current_thread = %d \n", 
         g_runnable_threads, g_current_thread);
    fflush(stdout);
    sem_post(&g_print_lock);
  }

  // This is just a shell it is not complete
  // It does nothing right now

  if (DEBUG) {
    sem_wait(&g_print_lock);
    INFO("EXITING PCT_thread_lock() - g_runnable_threads = %d - g_current_thread = %d \n", 
         g_runnable_threads, g_current_thread);
    fflush(stdout);
    sem_post(&g_print_lock);
  }

  sem_post(&PCT_lock);
  return;
}

void PCT_thread_unlock() {
  sem_t PCT_lock;
  sem_init(&PCT_lock, 0, 1);
  sem_wait(&PCT_lock);

  if (DEBUG) {
    sem_wait(&g_print_lock);
    INFO("ENTER PCT_thread_unlock() - g_runnable_threads = %d - g_current_thread = %d \n", 
         g_runnable_threads, g_current_thread);
    fflush(stdout);
    sem_post(&g_print_lock);
  }

  // This is just a shell it is not complete
  // It does nothing right now
  
  if (DEBUG) {
    sem_wait(&g_print_lock);
    INFO("EXITING PCT_thread_unlock() - g_runnable_threads = %d - g_current_thread = %d \n", 
         g_runnable_threads, g_current_thread);
    fflush(stdout);
    sem_post(&g_print_lock);
  }

  sem_post(&PCT_lock);
  return;
}

void PCT(int pct_thread_state) {
  sem_t PCT_lock;
  sem_init(&PCT_lock, 0, 1);
  sem_wait(&PCT_lock);

  if (DEBUG) {
    sem_wait(&g_print_lock);
    INFO("======================================================\n");
    fflush(stdout);
    sem_post(&g_print_lock);
  }

  if (pct_thread_state == PCT_INIT_MAIN) {
    // Create thread for main and give it a random priority
    PCT_init_main();
  } else if (pct_thread_state == PCT_THREAD_BEFORE_CREATE) {
    // Call before orig_pthread_create 
    PCT_thread_before_create();
  } else if (pct_thread_state == PCT_THREAD_AFTER_CREATE) {
    // Call after orig_pthread_create 
    PCT_thread_after_create();
  } else if (pct_thread_state == PCT_THREAD_START) {
    // Call when interpose start routine begins
    PCT_thread_start();
  } else if (pct_thread_state == PCT_THREAD_TERMINATE) {
    // Called when thread terminates or exit has been called
    PCT_thread_terminate();
  } else if (pct_thread_state == PCT_THREAD_YIELD) {
    // Called when pthread_yield is called
    PCT_thread_yield();
  } else if (pct_thread_state == PCT_THREAD_LOCK) {
    // Called when pthread_mutex_lock is called
    PCT_thread_lock();
  } else if (pct_thread_state == PCT_THREAD_UNLOCK) {
    // Called when pthread_mutex_unlock is called
    PCT_thread_unlock();
  } else {
    if (DEBUG) {
      sem_wait(&g_print_lock);
      INFO("PCT_DO_NOTHING\n");
      fflush(stdout);
      sem_post(&g_print_lock);
    }
  }
  // else if pct_thread_state == PCT_THREAD_DO_NOTHING

  if (DEBUG) {
    sem_wait(&g_print_lock);
    INFO("======================================================\n");
    fflush(stdout);
    sem_post(&g_print_lock);
  }

  sem_post(&PCT_lock);
  return;
}

////////////////////////////////////////////////////
/////////////// SCHEDULING ALGORITHMS //////////////
////////////////////////////////////////////////////

void run_scheduling_algorithm(int pct_thread_state) {
  int algorithm = get_algorithm_ID();
  if (algorithm == kAlgorithmRandom) {
    // run random scheduling algorithm
    rsleep();
  }
  else if (algorithm == kAlgorithmPCT) {
    // run pct scheduling algorithm
    PCT(pct_thread_state);
  }
  // else algorithm = none, do nothing special
}

////////////////////////////////////////////////////
////////////////////////////////////////////////////

// Thread Management
void *interpose_start_routine(void *argument) {
  // Deconstruct the struct into [ function to execute, arg ]
  sem_wait(&g_count_lock);
  struct arg_struct *arguments = argument;
  void *(*start_routine) (void *) = arguments->struct_func;
  void *arg = arguments->struct_arg;
  // Tell which semaphore this thread should wait on
  int thread_index = arguments->thread_index;
  sem_post(&g_count_lock);
  
  if (DEBUG) {
    sem_wait(&g_print_lock);
    INFO("ENTER interpose_start_routine() - thread_index = %d - thread_id = %ld - waiting on semaphore\n", 
         thread_index, g_threads[thread_index].thread_id);
    fflush(stdout);
    sem_post(&g_print_lock);
  }

  // Needs to tell pthread_start what semaphores to wait on
  sem_wait(&g_count_lock);
  g_current_thread = thread_index;
  sem_post(&g_count_lock);

  run_scheduling_algorithm(PCT_THREAD_START);

  if (DEBUG) {
    sem_wait(&g_print_lock);
    INFO("interpose_start_routine() - start_routine = %p\n", start_routine);
    fflush(stdout);
    sem_post(&g_print_lock);
  }

  sem_wait(&g_count_lock);
  g_thread_count++;
  g_thread_ids[g_thread_count] = gettid();
  sem_post(&g_count_lock);

  sem_wait(&g_print_lock);
  int thread_number = find_thread_number(gettid());
  INFO("THREAD CREATED (%d, %ld)\n", thread_number, gettid());
  fflush(stdout);
  sem_post(&g_print_lock);
  
  // Execute the function for the thread as normal
  void *return_val = start_routine(arg);

  // Need to tell PCT_thread_terminate() which thread is terminating
  g_current_thread = thread_index;
  run_scheduling_algorithm(PCT_THREAD_TERMINATE);

  sem_wait(&g_print_lock);
  INFO("THREAD EXITED (%d, %ld)\n", thread_number, gettid());
  fflush(stdout);
  sem_post(&g_print_lock);
  return return_val;
}

////////////////////////////////////////////////////
////////// BEGINNING OF PTHREAD FUNCTIONS //////////
////////////////////////////////////////////////////

int pthread_create(pthread_t *thread, const pthread_attr_t *attr,
                   void *(*start_routine) (void *), void *arg) {
  pthread_create_type orig_create;
  orig_create = (pthread_create_type)dlsym(RTLD_NEXT, "pthread_create");

  if (DEBUG) {
    sem_wait(&g_print_lock);
    INFO("======================================================\n");
    fflush(stdout);
    sem_post(&g_print_lock);
  }

  run_scheduling_algorithm(PCT_THREAD_BEFORE_CREATE);

  // Struct for multiple args
  struct arg_struct *args = malloc(sizeof(arg_struct));
  args->struct_func = start_routine;
  args->struct_arg = arg;
  // g_current_thread is set in PCT_thread_before_create() to be this thread
  // that is about to be created
  args->thread_index = g_current_thread;

  sem_wait(&g_print_lock);
  INFO("CALL pthread_create(%p, %p, %p, %p)\n", thread, attr, start_routine, arg);
  fflush(stdout);
  STACKTRACE_THREAD_ID = gettid();
  stacktrace();
  sem_post(&g_print_lock);

  int return_val = orig_create(thread, attr, &interpose_start_routine, (void *)args);

  run_scheduling_algorithm(PCT_THREAD_AFTER_CREATE);

  sem_wait(&g_print_lock);  
  INFO("RETURN pthread_create(%p, %p, %p, %p) = %d\n", thread, attr, start_routine, arg, return_val);
  fflush(stdout);
  sem_post(&g_print_lock);

  if (DEBUG) {
    sem_wait(&g_print_lock);
    INFO("======================================================\n");
    fflush(stdout);
    sem_post(&g_print_lock);
  }

  return return_val;
}

void pthread_exit(void *retval) {
  pthread_exit_type orig_exit;
  orig_exit = (pthread_exit_type)dlsym(RTLD_NEXT, "pthread_exit");

  if (DEBUG) {
    sem_wait(&g_print_lock);
    INFO("======================================================\n");
    fflush(stdout);
    INFO("ENTER pthread_exit - g_current_thread = %d\n", g_current_thread);
    fflush(stdout);
    sem_post(&g_print_lock);
  }

  run_scheduling_algorithm(PCT_DO_NOTHING);

  sem_wait(&g_print_lock);  
  INFO("CALL pthread_exit(%p)\n", retval);
  fflush(stdout);
  STACKTRACE_THREAD_ID = gettid();
  stacktrace();
  sem_post(&g_print_lock);

  run_scheduling_algorithm(PCT_THREAD_TERMINATE);

  sem_wait(&g_print_lock);
  int thread_number = find_thread_number(gettid());
  INFO("THREAD EXITED (%d, %ld)\n", thread_number, gettid());
  fflush(stdout);
  sem_post(&g_print_lock);

  if (DEBUG) {
    sem_wait(&g_print_lock);
    INFO("======================================================\n");
    fflush(stdout);
    sem_post(&g_print_lock);
  }

  orig_exit(retval);
  return;
}

int pthread_yield(void) {
  pthread_yield_type orig_yield;
  orig_yield = (pthread_yield_type)dlsym(RTLD_NEXT, "pthread_yield");

  if (DEBUG) {
    sem_wait(&g_print_lock);
    INFO("======================================================\n");
    fflush(stdout);
    sem_post(&g_print_lock);
  }

  run_scheduling_algorithm(PCT_DO_NOTHING);

  sem_wait(&g_print_lock);
  INFO("CALL pthread_yield()\n");
  fflush(stdout);
  STACKTRACE_THREAD_ID = gettid();
  stacktrace();
  sem_post(&g_print_lock);

  int return_val = orig_yield();

  run_scheduling_algorithm(PCT_THREAD_YIELD);

  sem_wait(&g_print_lock);  
  INFO("RETURN pthread_yield() = %d\n", return_val);
  fflush(stdout);
  sem_post(&g_print_lock);

  if (DEBUG) {
    sem_wait(&g_print_lock);
    INFO("======================================================\n");
    fflush(stdout);
    sem_post(&g_print_lock);
  }
  return return_val;
}

// Condition variables
int pthread_cond_wait(pthread_cond_t *cond, pthread_mutex_t *mutex) {
  pthread_cond_wait_type orig_cond_wait;
  orig_cond_wait = (pthread_cond_wait_type)dlsym(RTLD_NEXT, "pthread_cond_wait");

  run_scheduling_algorithm(PCT_THREAD_CALL);

  sem_wait(&g_print_lock);  
  INFO("CALL pthread_cond_wait(%p, %p)\n", cond, mutex);
  fflush(stdout);
  STACKTRACE_THREAD_ID = gettid();
  stacktrace();
  sem_post(&g_print_lock);

  int return_val = orig_cond_wait(cond, mutex);

  run_scheduling_algorithm(PCT_DO_NOTHING);

  sem_wait(&g_print_lock);  
  INFO("RETURN pthread_cond_wait(%p, %p) = %d\n", cond, mutex, return_val);
  fflush(stdout);
  sem_post(&g_print_lock);

  return return_val;
}

int pthread_cond_signal(pthread_cond_t *cond) {
  pthread_cond_signal_type orig_cond_signal;
  orig_cond_signal = (pthread_cond_signal_type)dlsym(RTLD_NEXT, "pthread_cond_signal");

  run_scheduling_algorithm(PCT_THREAD_CALL);

  sem_wait(&g_print_lock);
  INFO("CALL pthread_cond_signal(%p)\n", cond);
  fflush(stdout);
  STACKTRACE_THREAD_ID = gettid();
  stacktrace();
  sem_post(&g_print_lock);

  int return_val = orig_cond_signal(cond);

  run_scheduling_algorithm(PCT_DO_NOTHING);

  sem_wait(&g_print_lock);
  INFO("RETURN pthread_cond_signal(%p) = %d\n", cond, return_val);
  fflush(stdout);
  sem_post(&g_print_lock);

  return return_val;
}

int pthread_cond_broadcast(pthread_cond_t *cond) {
  pthread_cond_broadcast_type orig_cond_broadcast;
  orig_cond_broadcast = (pthread_cond_broadcast_type)dlsym(RTLD_NEXT, "pthread_cond_broadcast");
  
  run_scheduling_algorithm(PCT_THREAD_CALL);

  sem_wait(&g_print_lock);
  INFO("CALL pthread_cond_broadcast(%p)\n", cond);
  fflush(stdout);
  STACKTRACE_THREAD_ID = gettid();
  stacktrace();
  sem_post(&g_print_lock);

  int return_val = orig_cond_broadcast(cond);

  run_scheduling_algorithm(PCT_DO_NOTHING);

  sem_wait(&g_print_lock);
  INFO("RETURN pthread_cond_broadcast(%p) = %d\n", cond, return_val);
  fflush(stdout);
  sem_post(&g_print_lock);

  return return_val;
}

// Mutexes
int pthread_mutex_lock(pthread_mutex_t *mutex) {
  pthread_mutex_lock_type orig_mutex_lock;
  orig_mutex_lock = (pthread_mutex_lock_type)dlsym(RTLD_NEXT, "pthread_mutex_lock");
  
  if (STACKTRACE_THREAD_ID == gettid()) {
    // If this thread is currently printing the stacktrace, allow it to use the original function.
    return orig_mutex_lock(mutex);
  } 
  
  run_scheduling_algorithm(PCT_THREAD_CALL);

  sem_wait(&g_print_lock);
  INFO("CALL pthread_mutex_lock(%p)\n", mutex);
  fflush(stdout);
  STACKTRACE_THREAD_ID = gettid();
  stacktrace();
  sem_post(&g_print_lock);
  
  int return_val = orig_mutex_lock(mutex);

  run_scheduling_algorithm(PCT_DO_NOTHING);

  sem_wait(&g_print_lock);
  INFO("RETURN pthread_mutex_lock(%p) = %d\n", mutex, return_val);
  fflush(stdout);
  sem_post(&g_print_lock);

  return return_val;
}

int pthread_mutex_unlock(pthread_mutex_t *mutex) {
  pthread_mutex_unlock_type orig_mutex_unlock = NULL;
  orig_mutex_unlock = (pthread_mutex_unlock_type)dlsym(RTLD_NEXT, "pthread_mutex_unlock");

  if (STACKTRACE_THREAD_ID == gettid()) {
    // If this thread is currently printing the stacktrace, allow it to use the original function.
    return orig_mutex_unlock(mutex);
  }
  
  run_scheduling_algorithm(PCT_THREAD_CALL);

  sem_wait(&g_print_lock);
  INFO("CALL pthread_mutex_unlock(%p)\n", mutex);
  fflush(stdout);
  STACKTRACE_THREAD_ID = gettid();
  stacktrace();
  sem_post(&g_print_lock);

  int return_val = orig_mutex_unlock(mutex);

  run_scheduling_algorithm(PCT_DO_NOTHING);

  sem_wait(&g_print_lock);
  INFO("RETURN pthread_mutex_unlock(%p) = %d\n", mutex, return_val);
  fflush(stdout);
  sem_post(&g_print_lock);

  return return_val;
}

int pthread_mutex_trylock(pthread_mutex_t *mutex) {
  pthread_mutex_trylock_type orig_mutex_trylock;
  orig_mutex_trylock = (pthread_mutex_trylock_type)dlsym(RTLD_NEXT, "pthread_mutex_trylock");

  run_scheduling_algorithm(PCT_THREAD_CALL);

  sem_wait(&g_print_lock);
  INFO("CALL pthread_mutex_trylock(%p)\n", mutex);
  fflush(stdout);
  STACKTRACE_THREAD_ID = gettid();
  stacktrace();
  sem_post(&g_print_lock);

  int return_val = orig_mutex_trylock(mutex);

  run_scheduling_algorithm(PCT_DO_NOTHING);

  sem_wait(&g_print_lock);
  INFO("RETURN pthread_mutex_trylock(%p) = %d\n", mutex, return_val);
  fflush(stdout);
  sem_post(&g_print_lock);

  return return_val;
}

// This will get called at the start of the target programs main function
static __attribute__((constructor (200))) void init_testlib(void) {
  // Used in PCT
  g_orig_mutex_lock = (pthread_mutex_lock_type)dlsym(RTLD_NEXT, "pthread_mutex_lock");
  g_orig_mutex_unlock = (pthread_mutex_unlock_type)dlsym(RTLD_NEXT, "pthread_mutex_unlock");
  
  pthread_mutex_t init_lock;
  g_orig_mutex_lock(&init_lock);


  // You can initialize stuff here
  INFO("Testlib loaded!\n");
  fflush(stdout);
  INFO("Stacktraces is %i\n", get_stacktraces());
  fflush(stdout);
  INFO("Algorithm ID is %i\n", get_algorithm_ID());
  fflush(stdout);
  INFO("Seed is %i\n",(int) get_seed());
  fflush(stdout);


  // Initialize semaphores for thread count and print lock
  sem_init(&g_print_lock, 0, 1);
  sem_init(&g_count_lock, 0, 1);  

  // Needed for PCT
  g_threads = (struct thread_struct*) malloc(MAX_THREADS * sizeof(struct thread_struct));
  g_semaphores = (sem_t *) malloc(MAX_THREADS * sizeof(sem_t));
  g_thread_mutexes = (struct mutex_struct *) malloc(MAX_MUTEXES * sizeof(struct mutex_struct));

  for (int i = 0; i < MAX_THREADS; i++) {
    g_threads[i].state = THREAD_DEAD;
    // Initialize array of semaphores for PCT
    sem_init(&g_semaphores[i], 0, 0);

    g_thread_mutexes[i].mutex = NULL;
    g_thread_mutexes[i].num_threads_waiting = 0;
  }
  // May want to put a conditional statement around this if you are not running the PCT algorithm
  PCT(PCT_INIT_MAIN);

  g_orig_mutex_unlock(&init_lock);

}
