#define _GNU_SOURCE
#include <assert.h> // Use this, it is your friend!
#include <stdlib.h>
#include <string.h>
#include "testlib.h"
#include "utils.h"
#include "libaccess.h"
#include <dlfcn.h>
#include <unistd.h>
#include <stdbool.h>
#include <sys/syscall.h>

#define gettid() syscall(SYS_gettid)

#define MAX_THREAD 64
#define MAX_MEMORY_ADDRESSES 20

////////////////////////////// GLOBAL //////////////////////////////
// Starting exit code
int EXIT_CODE = 0; // REVISION

// Original pthread function types
typedef int (*pthread_create_type)();
typedef int (*pthread_mutex_lock_type)();
typedef int (*pthread_mutex_unlock_type)();
typedef int (*pthread_mutex_init_type)();

// Original pthread functions
pthread_create_type orig_create = NULL;
pthread_mutex_lock_type orig_mutex_lock = NULL;
pthread_mutex_unlock_type orig_mutex_unlock = NULL;
pthread_mutex_init_type orig_mutex_init = NULL;

// Total number of threads that are active
int g_thread_count = 0;

// Keeps track of thread counts - index is find_thread_number(gettid())
long int g_thread_ids[MAX_THREAD];

// Locks
pthread_mutex_t g_print_lock;   // around any INFO()
pthread_mutex_t g_global_lock;  // around any global data structures or variables

// Keep track of algorithm
bool HAPPENS_BEFORE = false;
bool LOCKSET = false;



typedef struct last_accesses_struct {
  m_access **last_accesses;
  int last_accesses_len;
} last_accesses_struct;

typedef struct candidate_set_struct {
  pthread_mutex_t ** candidate_set;
  int candidate_set_length;
} candidate_set_struct;

typedef struct intersection_set_struct {
  pthread_mutex_t ** intersection_set;
  int intersection_set_len;
} intersection_set_struct;


////////////////////////////// LOCKSET //////////////////////////////

// Array of memory accesses
m_access g_mem_addr[MAX_MEMORY_ADDRESSES];

// Set of held locks
pthread_mutex_t *g_held_locks[MAX_THREAD][MAX_LOCKS];   // each thread has its own mutex ptr array
// pthread_mutex_t **g_held_locks;

////////////////////////////// HAPPENS-BEFORE //////////////////////////////
// Vector Clock - one for each thread, indexed using find_thread_number( gettid() )
// vp[t] is the vector clock for thread with index t.
// Each vector clock has an index for every other thread.
// Each index in each thread's vector clock represents how many events are certified "happened-before" in relation to the current thread
// For example, in thread t, vp[t][x] is how many events that thread t knows FOR A FACT have happened in thread x
int vp[MAX_THREAD][MAX_THREAD] = { 0 };

// Array of mutexes declared - used to help keep track of messages for each mutex
// Corresponds to g_messages array
pthread_mutex_t *g_mutexes[MAX_LOCKS];

// Array with an index for each lock (MAX_LOCKS).
// Each MAX_THREAD index represents the message being sent from an unlock() operation to the next lock() operation
int g_messages[MAX_LOCKS][MAX_THREAD];


////////////////////////////////////////////////////
///////////////////// HELPERS //////////////////////
////////////////////////////////////////////////////


/*
 * Used to find the thread's thread number (assigned via g_thread_count)
 * Should ALWAYS be locked via g_global_lock
 */
int find_thread_number(long int tid) {
  int thread_num = -1;
  int thread_ids_size = sizeof(g_thread_ids) / sizeof(g_thread_ids[0]);
  for (int i = 0; i < thread_ids_size; i++) {
    if (g_thread_ids[i] == tid) {
      thread_num = i;
      break;
    }
  }
  assert(thread_num != -1);
  return thread_num;
}

/*
 * Increment the event counter for the current thread in the Lamport Vector Clock array
 * Should be used for every internal thread event (memory access or synchronization operation)
 * Used for Happens-Before algorithm
 * Must ALWAYS be locked with g_global_lock
 */
void increment_event_counter() {
  int thread_id = find_thread_number(gettid());
  vp[thread_id][thread_id]++;
}

/*
 * Prints the vector clock array for the current thread
 * Format:
 * Thread 1: [1, 5, 0]
 * Must ALWAYS be locked with g_global_lock
 */
void log_happens_before() {
  orig_mutex_lock(&g_print_lock);

  int thread_id = find_thread_number(gettid());

  INFO("Thread %d: [", thread_id);
  fflush(stdout);
  for (int i = 0; i < MAX_THREAD; i++) {
    if (i == MAX_THREAD - 1) {
      INFO("\b]\n");
      fflush(stdout);
      break;
    }
    INFO("%d,", vp[thread_id][i]);
    fflush(stdout);
  }
  
  orig_mutex_unlock(&g_print_lock);
}

/*
 * Synchronize current and new vectors on every message received.
 * vp[i] = MAX(  old[i], new[i]  ) for every i
 * Must ALWAYS be locked with g_global_lock
 */
void sync_vectors(int *new) {
  int thread_id = find_thread_number(gettid());
  int *old = vp[thread_id];
  int max = -1;
  for (int i = 0; i < MAX_THREAD; i++) {
    max = -1;
    if (old[i] > new[i]) {
      max = old[i];
    }
    else {
      max = new[i];
    }

    vp[thread_id][i] = max;
  }
}

/*
 * Check if intersection array is empty -> DATA RACE
 */
int get_size(void **array) {
  int size = 0;
  for (int i = 0; i < MAX_LOCKS; i++) {
    if (array[i] != NULL) {
      size++;
    }
  }
  return size;
}

/*
 * Create new m_access object
 */
m_access *create_mem_access(void *address, bool is_write, int size, int thread_seq,
                            void *locks_held, int locks_held_len, char *var_name, const char *location) {
  m_access *new_mem_access = malloc(sizeof(m_access));

  new_mem_access->address = address;
  new_mem_access->is_write = is_write;
  new_mem_access->size = size;
  new_mem_access->thread_seq = thread_seq;
  new_mem_access->locks_held_len = locks_held_len;

  memcpy(new_mem_access->locks_held, locks_held, locks_held_len * sizeof(void *));

  new_mem_access->variable_name = malloc(strlen(var_name) * sizeof(char));
  strcpy(new_mem_access->variable_name, var_name);

  new_mem_access->location = malloc(strlen(location) * sizeof(char));
  strcpy(new_mem_access->location, location);

  return new_mem_access;
}

/*
 * Finds the mutex in the global array 'g_mutexes'. If it's nonexistent, place it in the array.
 * Return the index of the mutex in the array 'g_mutexes'.
 * Must ALWAYS be locked with g_global_lock
 */
int get_mutex_index(pthread_mutex_t *mutex) {
  int mutex_index = -1;
  for (int i = 0; i < MAX_LOCKS; i++) {
    if (g_mutexes[i] == NULL) {
      g_mutexes[i] = mutex;
      mutex_index = i;
      break;
    }
    else if (g_mutexes[i] == mutex) {
      mutex_index = i;
      break;
    }
  }
  assert(mutex_index != -1);

  return mutex_index;
}

/*
 * Sends the current thread's vector clock as a message intended for the next thread that lock this mutex
 * Must ALWAYS be locked with g_global_lock
 */
void send_message(pthread_mutex_t *mutex) {
  // Get the index that the mutex object corresponds to in the g_messages array
  int mutex_index = get_mutex_index(mutex);
  int thread_index = find_thread_number(gettid());

  // Copy over the vector clock of the current thread to the message for the mutex
  for (int i = 0; i < MAX_THREAD; i++) {
    g_messages[mutex_index][i] = vp[thread_index][i];
  }
}

/*
 * Checks if there is a message queued for this mutex. If there is, sync the current vector clock with the received vector clock.
 * Must ALWAYS be locked with g_global_lock
 */
void receive_message(pthread_mutex_t *mutex) {
  // Get the index that the mutex object corresponds to in the g_messages array
  int mutex_index = get_mutex_index(mutex);

  // Synchronize the received vector and the current vector
  int *received_vector = g_messages[mutex_index];
  sync_vectors(received_vector);
}

/*
 * Insert memory access in g_mem_addr
 * If it's the first occurrence, return NULL
 * If it's NOT the first occurrence, return array of accesses as follows:
 *        If the current access is a write, return ALL previous accesses
 *        If the current access is a read, return ALL previous accesses that are a WRITE
 *        If nothing is found while looking, return NULL
 * Must ALWAYS be locked with g_global_lock
 */
struct last_accesses_struct *add_mem_access(void *addr, int access_size, const char *addr_name, const char *src_loc, bool is_write) {
  int thread_id = find_thread_number(gettid());

  struct last_accesses_struct *x = malloc(sizeof(struct last_accesses_struct));
  x->last_accesses = malloc(sizeof(m_access *) * MAX_MEMORY_ADDRESSES);
  int number_last_accesses = 0;

  for (int i = 0; i < MAX_MEMORY_ADDRESSES; i++) {
    
    if (g_mem_addr[i].address == addr) {
      // add to last_accesses
      if (!is_write && !g_mem_addr[i].is_write) {
        // read-read --> no race condition possible
        if (LOCKSET) {
          // If evaluating lockset, add it anyways. Need to keep track of all previous accesses
          x->last_accesses[number_last_accesses] = &g_mem_addr[i];
          number_last_accesses++;
        }
        continue;
      }
      else {
        // read-write, write-read, or write-write --> race condition possible
        x->last_accesses[number_last_accesses] = &g_mem_addr[i];
        number_last_accesses++;
      }
    }
    else if (g_mem_addr[i].address == NULL) {
      // add to g_mem_addr and break
      m_access *new_access = create_mem_access(addr, is_write, access_size, find_thread_number(gettid()), (void *) g_held_locks[thread_id], get_size((void **)g_held_locks[thread_id]), (void *)addr_name, src_loc);
      g_mem_addr[i] = *new_access;
      break;
    }
  }

  x->last_accesses_len = number_last_accesses;

  return x;
}

/*
 * Lockset version of add_mem_access
 * TODO: return EVERYTHING, not just possible conflicts
 */
m_access **add_mem_access_lockset(void *addr, int access_size, const char *addr_name, const char *src_loc, bool is_write) {
  int thread_id = find_thread_number(gettid());

  m_access ** last_accesses = malloc(sizeof(m_access *) * MAX_MEMORY_ADDRESSES);
  int number_last_accesses = 0;

  for (int i = 0; i < MAX_MEMORY_ADDRESSES; i++) {
    if (g_mem_addr[i].address == addr) {
      // add to last_accesses
      *last_accesses[number_last_accesses] = g_mem_addr[i];
      number_last_accesses++;
    }
    else if (g_mem_addr[i].address == NULL) {
      // add to g_mem_addr and break
      m_access *new_access = create_mem_access(addr, is_write, access_size, find_thread_number(gettid()), (void *) g_held_locks[thread_id], get_size((void **)g_held_locks[thread_id]), (void *)addr_name, src_loc);
      g_mem_addr[i] = *new_access;
      break;
    }
  }

  if (number_last_accesses == 0) {
    return NULL;
  }

  return last_accesses;
}

/*
 * Finds the intersection of the candidate set and the locks held set and returns
 * the candidate set size
 */

int find_intersection(void **candidate_set, void **intersection_set) {
  int thread_id = find_thread_number(gettid());

  int intersection_set_len = 0;
  for (int i = 0; i < MAX_LOCKS; i++) {
    for (int j = 0; j < MAX_LOCKS; j++) {
      if (g_held_locks[thread_id][i] != NULL && g_held_locks[thread_id][i] == candidate_set[j]) {
        ((pthread_mutex_t **) intersection_set)[intersection_set_len] = g_held_locks[thread_id][i];
        intersection_set_len++;
      }
    }
  }

  return intersection_set_len;
}

/*
 * Find the intersection between two arrays of mutexes
 */
struct intersection_set_struct *find_intersection_set(pthread_mutex_t **held_locks_one, pthread_mutex_t **held_locks_two) {
  struct intersection_set_struct *intersections = malloc(sizeof(intersection_set_struct));
  intersections->intersection_set = malloc(sizeof(pthread_mutex_t *) * MAX_LOCKS);

  int intersection_set_len = 0;

  // Find intersections between held_locks_one and held_locks_two
  for (int i= 0; i < MAX_LOCKS; i++) {
    for (int j = 0; j < MAX_LOCKS; j++) {
      if (held_locks_one[i] != NULL && held_locks_one[i] == held_locks_two[j]) {
        intersections->intersection_set[intersection_set_len] = held_locks_one[i];
        intersection_set_len++;
      }
    }
  }

  intersections->intersection_set_len = intersection_set_len;
  return intersections;
}

/*
 * Finds the candidate set from the set of all last accesses to the memory location
 */
struct candidate_set_struct *find_candidate_set(struct last_accesses_struct *x) {
  m_access **last_accesses = x->last_accesses;
  int last_accesses_len = x->last_accesses_len;

  int thread_id = find_thread_number(gettid());

  // Find the intersection between the g_held_locks[thread_id] and every previous access
  struct intersection_set_struct *intersections = malloc(sizeof(intersection_set_struct));
  // TODO: malloc? intersections->intersection_set
  intersections = find_intersection_set((pthread_mutex_t **)last_accesses[0]->locks_held, g_held_locks[thread_id]);

  for (int i = 1; i < last_accesses_len; i++) {
    intersections = find_intersection_set((pthread_mutex_t **)last_accesses[i]->locks_held, intersections->intersection_set);
  }

  // initialize the candidate set struct
  struct candidate_set_struct *candidates = malloc(sizeof(struct candidate_set_struct));
  candidates->candidate_set = malloc(sizeof(m_access *) * MAX_MEMORY_ADDRESSES);
  candidates->candidate_set = intersections->intersection_set;
  candidates->candidate_set_length = intersections->intersection_set_len;

  return candidates;
}

/*
 * Given the most recent memory access and the current memory access, determine if there is a race condition
 * by (1) checking if the intersection of held locks and the candidate set is empty and (2) checking if either
 * access is a write 
 * Must ALWAYS be locked with g_global_lock
 */
void verify_lockset(void * addr, int access_size, const char *addr_name, const char *src_loc, bool is_write, int thread_id) {
  printf("Verifying lockset\n");
  fflush(stdout);

  struct last_accesses_struct *x = add_mem_access(addr, access_size, addr_name, src_loc, is_write);

  // Break down struct
  m_access **last_accesses = x->last_accesses;
  int last_accesses_len = x->last_accesses_len;

  printf("LAST ACCESSES (len = %d)\n", last_accesses_len);
  fflush(stdout);

  for (int i = 0; i < last_accesses_len; i++) {
    printf("  %p\n", last_accesses[i]);
    fflush(stdout);
  }

  if (last_accesses_len == 0) {
    // first time that this memory location is accessed OR no previous accesses that would cause race condition
    return;
  }

  m_access *curr_memory_access = NULL;
  int curr_memory_access_index = -1;
  for (int i = MAX_MEMORY_ADDRESSES - 1; i >= 0; i--) {
    // Find the memory access just inserted into g_mem_addr
    if (g_mem_addr[i].address != NULL) {
      curr_memory_access = &g_mem_addr[i];
      curr_memory_access_index = i;
      break;
    }
  }

  assert(curr_memory_access != NULL);     // Should NEVER be this
  assert(curr_memory_access_index >= 0);  // Should NEVER be this

  // Right now, we have:
  //    curr_memory_access        --> m_addr struct in g_mem_addr that we just inserted
  //    curr_memory_access_index  --> index where we just inserted curr_memory_access
  //    last_accesses_struct         --> array of previous memory accesses on this var

  // Get candidate set from intersection of all last accesses
  struct candidate_set_struct *candidate_set = find_candidate_set(x);

  printf("CANDIDATE SET FOUND! (len %d)\n", candidate_set->candidate_set_length);
  fflush(stdout);
  for (int i = 0; i < candidate_set->candidate_set_length; i++) {
    printf("  %p\n", candidate_set->candidate_set[i]);
    fflush(stdout);
  }

  if (candidate_set->candidate_set_length == 0) {
    // Race condition possible! 
    if (is_write) {
      // If currently writing, find last access.
      orig_mutex_lock(&g_print_lock);
      log_race(curr_memory_access, x->last_accesses[x->last_accesses_len - 1]);
      EXIT_CODE = 1;    // REVISION
      orig_mutex_unlock(&g_print_lock);
    }
    else {
      // If currently reading, find last write access.
      for (int i = x->last_accesses_len - 1; i >= 0; i--) {
        if (x->last_accesses[i]->is_write) {
          // Found the most recent write. Report as data race
          orig_mutex_lock(&g_print_lock);
          log_race(curr_memory_access, x->last_accesses[i]);
          EXIT_CODE = 1;
          orig_mutex_unlock(&g_print_lock);
          break;
        }
      }
    }
  }
}

/*
 * Takes in the data for a variable's memory address.
 * Verifies whether or not the happens-before relation holds between the current thread and previous thread that accessed the memory location
 * Must ALWAYS be locked with g_global_lock
 */
void verify_happens_before(void *addr, int access_size, const char *addr_name, const char *src_loc, bool is_write) {
  printf("Verifying happens-before\n");
  fflush(stdout);

  struct last_accesses_struct *x = add_mem_access(addr, access_size, addr_name, src_loc, is_write);

  // Break down struct
  m_access **last_accesses = x->last_accesses;
  int last_accesses_len = x->last_accesses_len;

  printf("LAST ACCESSES (len = %d)\n", last_accesses_len);
  fflush(stdout);

  for (int i = 0; i < last_accesses_len; i++) {
    printf("  %p\n", last_accesses[i]);
    fflush(stdout);
  }




  if (last_accesses_len == 0) {
    // first time that this memory location is accessed OR no previous accesses that would cause race condition
    return;
  }

  // thread id for the current thread that accessed the variable
  int curr_access = find_thread_number(gettid());

  m_access *curr_memory_access = NULL;
  int curr_memory_access_index = -1;
  for (int i = MAX_MEMORY_ADDRESSES - 1; i >= 0; i--) {
    // Find the memory access just inserted into g_mem_addr
    if (g_mem_addr[i].address != NULL) {
      curr_memory_access = &g_mem_addr[i];
      curr_memory_access_index = i;
      break;
    }
  }

  assert(curr_memory_access != NULL);     // Should NEVER be this
  assert(curr_memory_access_index >= 0);  // Should NEVER be this

  // Right now, we have:
  //    curr_access               --> thread_id
  //    curr_memory_access        --> m_addr struct in g_mem_addr that we just inserted
  //    curr_memory_access_index  --> index where we just inserted curr_memory_access
  //    last_accesses_struct         --> array of previous memory accesses on this var that could cause race condition

  for (int i = last_accesses_len - 1; i >= 0; i--) {
    // Go in reverse from the just-inserted mem access in order to find last occurrence that is race condition

    // thread id for the previous thread that accessed the variable
    int prev_access = last_accesses[i]->thread_seq;

    // Check if there could be a data race
    if ((!last_accesses[i]->is_write &&  !is_write) ||  // read-read
        (curr_access == prev_access)) {                  // last access also by this thread
      // no possible data race!
      INFO("NO DATA RACE POSSIBLE!\n");
      fflush(stdout);
    }
    else {  // write-write, write-read, read-write, AND last access by a different thread
      // possible data race
      // CHECK IF HAPPENS-BEFORE HOLDS
      if (vp[prev_access][prev_access] <= vp[curr_access][prev_access]) {
        // The previous thread synchronized its events with the current thread
        // HAPPENS-BEFORE HOLDS
        INFO("Happens-Before HOLDS\n");
        fflush(stdout);
      }
      else if (vp[prev_access][prev_access] > vp[curr_access][prev_access]) {
        // The previous thread did NOT synchronize its events with the current thread
        // HAPPENS-BEFORE DOES NOT HOLD
      
        // Potentially conflicting access
        m_access *conflicting_access = malloc(sizeof(m_access));
        conflicting_access = last_accesses[i];

        orig_mutex_lock(&g_print_lock);
        log_race(curr_memory_access, conflicting_access);
        EXIT_CODE = 1;
        orig_mutex_unlock(&g_print_lock);
        break;  // Only need to spit out the first race condition found
      }
    }
  }
}


////////////////////////////////////////////////////
////////////////// THREAD CREATION /////////////////
////////////////////////////////////////////////////

// Used for intercept_start_routine in order to pass multiple args
typedef struct arg_struct {
  void *(*func) (void *);   // Function pointer for original start_routine
  void *arg;                // void * arg passed to pthread_create
  int *vector;              // Parent's Happens-Before vector
} arg_struct;

/*
 * Intercepts the function run by pthread_create in order to keep track of it
 */
void *intercept_start_routine(void *argument) {
  // Deconstruct the struct into [ function to execute, arg, received_vector ]
  struct arg_struct *arguments = argument;
  void *(*start_routine) (void *) = arguments->func;
  void *arg = arguments->arg;
  int *received_vector = arguments->vector;

  // Assign a unique thread id to the newly created thread
  orig_mutex_lock(&g_global_lock);
  g_thread_count++;
  g_thread_ids[g_thread_count] = gettid();
  orig_mutex_lock(&g_print_lock);
  log_thread_created(g_thread_count);     // Prints out thread created statement
  orig_mutex_unlock(&g_print_lock);
  orig_mutex_unlock(&g_global_lock);

  if (HAPPENS_BEFORE) {
    orig_mutex_lock(&g_global_lock);
    // This causes the child to have the same vector clock that the parent has.
    // This is because the child is guaranteed to happen-after every event that the parent has happened-after
    sync_vectors(received_vector);
    log_happens_before();
    orig_mutex_unlock(&g_global_lock);
  }

  // Run the original function meant to be ran and return the return value
  void *return_val = start_routine(arg);

  return return_val;
}

/*
 * Creates a thread
 */
int pthread_create(pthread_t *thread, const pthread_attr_t *attr,
                   void *(*start_routine) (void *), void *arg) {
  orig_mutex_lock(&g_global_lock);
  // Since this is a synchronization operation, increment event counter
  increment_event_counter();

  int thread_id = find_thread_number(gettid());
  // Store the real function pointer and the argument passed into a struct
  struct arg_struct *args = malloc(sizeof(arg_struct));
  args->func = start_routine;
  args->arg = arg;
  args->vector = malloc(sizeof(int) * MAX_THREAD);
  *args->vector = *vp[thread_id];
  orig_mutex_unlock(&g_global_lock);

  orig_mutex_lock(&g_print_lock);
  INFO("Called pthread_create\n");
  fflush(stdout);
  orig_mutex_unlock(&g_print_lock);

  if (HAPPENS_BEFORE) {
    // Print log for happens-before
    orig_mutex_lock(&g_global_lock);
    log_happens_before();
    orig_mutex_unlock(&g_global_lock);
  }
  // Redirect to intercept_start_routine instead of start_routine
  int return_val = orig_create(thread, attr, &intercept_start_routine, (void *)args);
  return return_val;
}



////////////////////////////////////////////////////
////////////////////// MUTEXES /////////////////////
////////////////////////////////////////////////////
/*
 * Atomically locks the mutex object referenced by 'mutex'
 * "Synchronization operation"
 */
int pthread_mutex_lock(pthread_mutex_t *mutex) {
  orig_mutex_lock(&g_print_lock);
  INFO("Called pthread_mutex_lock\n");
  fflush(stdout);
  orig_mutex_unlock(&g_print_lock);

  // Call the original lock function
  int return_val = orig_mutex_lock(mutex);

  // Putting this after the orig_mutex_lock function ensures that no deadlocks occur
  orig_mutex_lock(&g_global_lock);

  int thread_id = find_thread_number(gettid());

  // Add mutex to set of held locks
  for (int i = 0; i < MAX_LOCKS; i++) {
    if (g_held_locks[thread_id][i] == NULL) {
      g_held_locks[thread_id][i] = mutex;
      break;
    }
  }

  if (HAPPENS_BEFORE) {
    // Check for a message (vector clock) from the thread who previously unlocked this mutex, then sync the current and received vector clock
    receive_message(mutex);
    // Increment current thread's "event" counter
    increment_event_counter();
    // Print log for happens-before
    log_happens_before();
  }

  orig_mutex_unlock(&g_global_lock);
  return return_val;
}

/*
 * Atomically unlocks the mutex object referenced by 'mutex'
 * "Synchronization operation"
 */
int pthread_mutex_unlock(pthread_mutex_t *mutex) {
  orig_mutex_lock(&g_global_lock);

  orig_mutex_lock(&g_print_lock);
  INFO("Called pthread_mutex_unlock\n");
  fflush(stdout);
  orig_mutex_unlock(&g_print_lock);


  // Call the original unlock function
  int return_val = orig_mutex_unlock(mutex);

  int thread_id = find_thread_number(gettid());

  // Remove mutex from set of held locks
  for (int i = 0; i < MAX_LOCKS; i++) {
    if (g_held_locks[thread_id][i] == mutex) {
      g_held_locks[thread_id][i] = NULL;
      break;
    }
  }

  if (HAPPENS_BEFORE) {
    // Increment current thread's "event" counter
    increment_event_counter();
    // Print log for happens-before
    log_happens_before();
    // Send a message (current vector clock) intended for whichever thread locks 'mutex' next
    send_message(mutex);
  }

  orig_mutex_unlock(&g_global_lock);
  return return_val;
}


////////////////////////////////////////////////////
//////////////// READ/WRITE INTERCEPT //////////////
////////////////////////////////////////////////////
/*
 * Intercepts all write accesses, used to keep track of write accesses for lockset and happens-before
 */
extern void dc_write_handle(void * addr, int access_size, const char *addr_name, const char *src_loc){
  orig_mutex_lock(&g_global_lock);

  orig_mutex_lock(&g_print_lock);
  INFO("Calling overwritten write handler\n");
  fflush(stdout);
  INFO("Access addr: %p\nAccess Size:%d\n", addr, access_size);
  fflush(stdout);
  INFO("Variable Name: %s\nLocation: %s\n", addr_name, src_loc);
  fflush(stdout);
  orig_mutex_unlock(&g_print_lock);

  int thread_id = find_thread_number(gettid());

  if (HAPPENS_BEFORE) {
    // Increment current thread's "event" counter
    increment_event_counter();
    // Print log for happens-before
    log_happens_before();
    // Check for a data race in the thread which previously accessed the variable
    verify_happens_before(addr, access_size, addr_name, src_loc, true);
  }
  else if (LOCKSET) {
    // Check for data race using lockset algorithm
    verify_lockset(addr, access_size, addr_name, src_loc, true, thread_id);
  }
  orig_mutex_unlock(&g_global_lock);
}

/*
 * Intercepts all read accesses, used to keep track of read accesses for lockset and happens-before
 */
extern void dc_read_handle(void * addr, int access_size, const char*addr_name, const char *src_loc){
  orig_mutex_lock(&g_global_lock);

  orig_mutex_lock(&g_print_lock);
  INFO("Calling overwritten read handler\n");
  fflush(stdout);
  INFO("Access addr: %p\nAccess Size:%d\n", addr, access_size);
  fflush(stdout);
  INFO("Variable Name: %s\nLocation: %s\n", addr_name, src_loc);
  fflush(stdout);
  orig_mutex_unlock(&g_print_lock);

  int thread_id = find_thread_number(gettid());

  if (HAPPENS_BEFORE) {
    // Increment current thread's "event" counter
    increment_event_counter();
    // Print log for happens-before
    log_happens_before();
    // Check for a data race in the thread which previously accessed the variable
    verify_happens_before(addr, access_size, addr_name, src_loc, false);
  }
  else if (LOCKSET) {
    // Check for data race using lockset algorithm
    verify_lockset(addr, access_size, addr_name, src_loc, false, thread_id);
  }
  orig_mutex_unlock(&g_global_lock);
}



////////////////////////////////////////////////////
//////////////////// CONSTRUCTOR ///////////////////
////////////////////////////////////////////////////
/*
 * This will get called when the program terminates
 */
void exit_function() {
  _exit(EXIT_CODE);
}

/*
 * This will get called at the start of the target programs main function
 */
int initialized = 0;
static __attribute__((constructor (200))) void init_testlib(void) {

  if (!initialized) {

    INFO("Testlib loaded!\n");
    fflush(stdout);
    INFO("Algorithm ID is %i\n", get_algorithm_ID());
    fflush(stdout);

    initialized = 1;
    ////////////////////////////// GLOBAL //////////////////////////////
    // Determines which algorithm is running
    if (get_algorithm_ID() == kAlgorithmLockset) {
      LOCKSET = true;
    }
    else {
      HAPPENS_BEFORE = true;
    }

    // Find original pthread functions
    orig_create = (pthread_create_type)dlsym(RTLD_NEXT, "pthread_create");
    orig_mutex_lock = (pthread_mutex_lock_type)dlsym(RTLD_NEXT, "pthread_mutex_lock");
    orig_mutex_unlock = (pthread_mutex_unlock_type)dlsym(RTLD_NEXT, "pthread_mutex_unlock");
    orig_mutex_init = (pthread_mutex_init_type)dlsym(RTLD_NEXT, "pthread_mutex_init");

    // Initialize locks
    orig_mutex_init(&g_print_lock, NULL);
    orig_mutex_init(&g_global_lock, NULL);

    // Initialize main thread as g_thread_ids[0]
    g_thread_ids[g_thread_count] = gettid();


    ////////////////////////////// LOCKSET //////////////////////////////
    // Initialize global arrays

    ////////////////////////// HAPPENS-BEFORE ///////////////////////////
    // Initialize vector clock array. Main thread initialized with all 0's. Every other thread index is invalid, initialized with -1's
    for (int i = 0; i < MAX_THREAD; i++) {
      for (int j = 0; j < MAX_THREAD; j++) {
        if (i == 0) {
          // Main thread initialized to 0
          vp[i][j] = 0;
        }
        else {
          // Any other threads initialized to -1
          vp[i][j] = -1;
        }
      }
    }

    // Initialize g_messages array. Everything starts out as -1, since there are no messages
    for (int i = 0; i < MAX_LOCKS; i++) {
      for (int j = 0; j < MAX_THREAD; j++) {
        g_messages[i][j] = -1;
      }
    }
  }

  // REVISION 12-3-20
  atexit(exit_function);
}