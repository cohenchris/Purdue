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

// For logging purposes you should replace calls to printf(...) with calls to INFO(...)
// This makes it easy to enable or disable the output.
#ifndef NO_LOGGING
#define INFO(...) printf(__VA_ARGS__)
#else
#define INFO(f_, ...) ((void) 0)
#endif

// IDs for the 3 algorithms you must implement
static const int kAlgorithmNone = 0;
static const int kAlgorithmRandom = 1;
static const int kAlgorithmPCT = 2;

// get the algorithm id from the environment variables
int get_algorithm_ID();

// returns an array with 64 unique priority values based on the seed
int* get_priorities();

// returns the boolean (0,1) environment variable for stacktraces
int get_stacktraces();

// get seed set by environment
uint64_t get_seed();

// Sleep for random interval up to 1 second
void rsleep();

// Sleep for random interval up to param mod ms
// Expects mod >= 1
void rsleep_with_arg(int mod);

#endif
