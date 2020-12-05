/*
 * DO NOT MODIFY THIS FILE
 */

#include <assert.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "utils.h"

int string_equal(const char* str1, const char* str2) {
  return strcmp(str1, str2) == 0;
}

int get_algorithm_ID() {
  char * algorithm_var = getenv("ALGORITHM");
  if (string_equal(algorithm_var, "lockset"))
    return kAlgorithmLockset;
  else if (string_equal(algorithm_var, "happens"))
    return kAlgorithmHappens;
  else
    return -1;
}

void log_thread_created(int thread_seq) {
  printf("Thread %d was created\n", thread_seq);
}

// a1: conflicting memory access that was last executed
// a2: the other conflicting memory access
void log_race(m_access *a1, m_access *a2)
{
  printf("\n");

  printf("Possible data race during %s of size %d at %p by thread %d\n",
         a1->is_write?"write":"read", a1->size, a1->address, a1->thread_seq);
  printf("Variable: \"%s\"\n", a1->variable_name); 
  printf("Locks held:");
  assert(a1->locks_held_len <= MAX_LOCKS);
  if(a1->locks_held_len == 0){
    printf(" none\n");
  }else{
    int i;
    for(i=0;i<a1->locks_held_len;i++){
      printf(" %p", a1->locks_held[i]);
    }
    printf("\n");
  }
  printf("    at %s\n",a1->location);

  printf("\n");

  printf("This conflicts with a previous %s of size %d by thread %d\n",
         a2->is_write?"write":"read", a2->size, a2->thread_seq);
  printf("Variable: \"%s\"\n", a2->variable_name);
  printf("Locks held:");
  assert(a2->locks_held_len <= MAX_LOCKS);
  if(a2->locks_held_len == 0){
    printf(" none\n");
  }else{
    int i;
    for(i=0;i<a2->locks_held_len;i++){
      printf(" %p", a2->locks_held[i]);
    }
    printf("\n");
  }
  printf("    at %s\n",a2->location);


}
// This will get called before any other code.
static __attribute__((constructor (150))) void init_utils(void) {

#ifdef GRADING
#endif
}
