#include "main.h"

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <signal.h>
#include <sys/wait.h>

#include "server.h"
#include "socket.h"

#define DEFAULT_PORT (2450)
#define UNUSED(x) (void)(x)

/*
 * Handles SIGCHLD signals (zombie processes)
 */

extern void handle_sigchld(int sig, siginfo_t * info, void * ucontext) {
  UNUSED(sig);
  UNUSED(info);
  UNUSED(ucontext);

  /* The first argument of waitpid() being -1 means it waits for any child
   * process, rather than a specific one. The NULL second argument means it
   * doesn't store any exit information about the process into any variable. The
   * WNOHANG makes it so that the program doesn't hang while waiting for any
   * background process to exit.
   */

  while (waitpid(-1, NULL, WNOHANG) >= EXIT_SUCCESS) {
    continue;
  }

} /* handle_sigchld() */

/*
 * Print the usage for the program. Takes the name of the program as an
 * argument.
 */

void print_usage(char *name) {
  printf("USAGE: %s [-f|-t|-pNUM_THREADS] [-h] PORT_NO\n", name);
} /* print_usage() */

/*
 * Runs the webserver.
 */

int main(int argc, char **argv) {
  /* Signal handler for SIGCHLD (zombie processes) */

  struct sigaction zombie_struct = { 0 };
  zombie_struct.sa_flags = SA_RESTART;
  zombie_struct.sa_sigaction = handle_sigchld;
  sigemptyset(&zombie_struct.sa_mask);

  if (sigaction(SIGCHLD, &zombie_struct, NULL)) {
    perror("sigchld");
    exit(EXIT_FAILURE);
  }

  // default to linear

  concurrency_mode mode = E_NO_CONCURRENCY;

  int port_no = 0;
  int num_threads = 0;

  int c = 0;
  while ((c = getopt(argc, argv, "hftp:")) != -1) {
    switch (c) {
      case 'h':
        print_usage(argv[0]);
        return 0;
      case 'f':
        mode = E_FORK_PER_REQUEST;
        break;
      case 't':
        if (mode != E_NO_CONCURRENCY) {
          fputs("Multiple concurrency modes specified\n", stdout);
          print_usage(argv[0]);
          return -1;
        }
        mode = E_THREAD_PER_REQUEST;
        break;
      case 'p':
        if (mode != E_NO_CONCURRENCY) {
          fputs("Multiple concurrency modes specified\n", stdout);
          print_usage(argv[0]);
          return -1;
        }
        mode = (concurrency_mode) c;
        if (mode == E_THREAD_POOL) {
          num_threads = atoi(optarg);
        }
        break;
      case '?':
        if (isprint(optopt)) {
          fprintf(stderr, "Unknown option: -%c\n", optopt);
        }
        else {
          fprintf(stderr, "Unknown option\n");
        }
        // Fall through
      default:
        print_usage(argv[0]);
        return 1;
    }
  }

  if (optind > argc) {
    fprintf(stderr, "Extra arguments were specified");
    print_usage(argv[0]);
    return 1;
  }
  else if (optind == argc) {
    // use default port

    port_no = DEFAULT_PORT;
  }
  else {
    port_no = atoi(argv[optind]);
  }

  printf("%d %d %d\n", mode, port_no, num_threads);

  acceptor *acceptor = create_socket_acceptor(port_no);

  if (acceptor == NULL) {
	  return 1;
  }

  switch (mode) {
    case E_FORK_PER_REQUEST:
      run_forking_server(acceptor);
      break;
    case E_THREAD_PER_REQUEST:
      run_threaded_server(acceptor);
      break;
    case E_THREAD_POOL:
      run_thread_pool_server(acceptor, num_threads);
      break;
    default:
      run_linear_server(acceptor);
      break;
  }

  close_socket_acceptor(acceptor);

  return 0;
} /* main() */
