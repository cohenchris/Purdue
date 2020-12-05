/*
 * These are helper functions intended to simplify your life.
 * YOU DO NOT NEED TO MODIFY THIS FILE.
 * If you need further utility functions for the library add them directly in the source.
 * If you need further utility functions for your test programs, create a new utils source and header.
 */
#ifndef UTILS_H
#define UTILS_H

#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>

// For logging purposes you should replace calls to printf(...) with calls to INFO(...)
// This makes it easy to enable or disable the output.
// This will not be required for logging in this project, but I've included it so you
// Can use it for debugging, and enable or disable debugging output easily.
#ifndef NO_LOGGING
#define INFO(...) printf(__VA_ARGS__)
#else
#define INFO(f_, ...) ((void) 0)
#endif

#define MAX_LOCKS 10
typedef struct memory_memory{
  void * address;
  bool is_write;
  int size;
  int thread_seq;
  void* locks_held[MAX_LOCKS];
  int locks_held_len;
  char * variable_name;
  char* location;
} m_access;

// IDs for the 3 algorithms you must implement
static const int kAlgorithmLockset = 0;
static const int kAlgorithmHappens = 1;

// get the algorithm id from the environment variables
int get_algorithm_ID();

void log_thread_created(int);
void log_race(m_access *, m_access *);

#endif
