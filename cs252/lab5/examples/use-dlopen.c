#include <dlfcn.h>
#include <link.h>
#include <errno.h>
#include <stdio.h>
#include<stdlib.h>

// Typedef a function pointer for httprun
typedef void (*httprunfunc) (int ssock, const char *query_string);

/*
 * Sample program that uses dlopen and dlsym
 */

int main(int argc, char **argv) {
  // Opening 
  void *lib = dlopen("./hello.so", RTLD_LAZY);

  if (lib == NULL) {
    fprintf(stderr, "./hello.so not found\n");
    perror("dlopen");
    exit(1);
  }

  // Get function to print hello, defined in hello.c
  httprunfunc hello_httprun;

  // Use dlsym to handle opened lib
  hello_httprun = (httprunfunc) dlsym(lib, "httprun");
  if (hello_httprun == NULL) {
    perror("dlsym: httprun not found:");
    exit(1);
  }

  // Call the function
  hello_httprun(1, "a=b&c=d");
}

