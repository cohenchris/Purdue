#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>


/*
 *  Main function which forks a process to call the specified filename
 *  and returns its exit code
 */

int main(int argc, char** argv) {
  if (argc != 2) {
    return 0;
  }

  const char* filename = argv[1];
  pid_t pid = fork();

  if (pid == -1) {
    // Fork error
    return -1;
  }

  if (pid == 0) {
    // Child Process

    execl(filename, filename, NULL); // execute the binary
    exit(1);

  } else {
    // parent process
    int stat_val = 0;
    if (pid == (waitpid(pid, &stat_val, 0))) {
      if (WIFEXITED(stat_val)) {
        // return exit status of the executed binary
        return WEXITSTATUS(stat_val);
      }
    } else {
      // Waitpid error
      return -1;
    }
  }
} /* main() */
