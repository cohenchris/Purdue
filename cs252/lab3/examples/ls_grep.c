
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

const char *g_usage = ""
"Usage:\n"
"    ls_grep path word outfile\n"
"\n"
"    It does something similar to the shell command:\n"
"        csh> ls path | grep word > outfile\n"
"\n"
"Example:\n"
"    ls_grep ../ command out\n"
"    cat out\n\n";

const char *g_ls = "ls";
const char *g_grep = "grep";

/*
 *  Main function which does exactly what g_usage explains
 */

int main(int argc, char **argv) {
  if (argc < 4) {
    fprintf(stderr, "%s", g_usage);
    exit(1);
  }

  // Save default input, output, and error because we will
  // change them during redirection and we will need to restore them
  // at the end.

  int default_in = dup(0);
  int default_out = dup(1);
  int default_err = dup(2);

  //////////////////  ls  //////////////////////////

  // Input:    defaultin
  // Output:   pipe
  // Error:    defaulterr

  // Create new pipe

  int fdpipe[2];
  if (pipe(fdpipe) == -1) {
    perror("ls_grep: pipe");
    exit(2);
  }

  // Redirect input
  dup2(default_in, 0);

  // Redirect output to pipe
  dup2(fdpipe[1], 1);

  // Redirect err
  dup2(default_err, 2);

  // Create new process for "ls"
  int pid = fork();
  if (pid == -1) {
    perror("ls_grep: fork\n");
    exit(2);
  }

  if (pid == 0) {
    //Child

    // close file descriptors that are not needed
    close(fdpipe[0]);
    close(fdpipe[1]);
    close(default_in);
    close(default_out);
    close(default_err);

    // You can use execvp() instead if the arguments are stored in an array
    execlp(g_ls, g_ls, argv[1], NULL);

    // exec() is not suppose to return, something went wrong
    perror("ls_grep: exec ls");
    exit(2);
  }

  //////////////////  grep //////////////////////////

  // Input:    pipe
  // Output:   outfile
  // Error:    defaulterr

  // Redirect input.
  dup2(fdpipe[0], 0);

  // Redirect output to out_file
  int out_fd = creat(argv[3], 0600);

  if (out_fd < 0) {
    perror("ls_grep: creat outfile");
    exit(2);
  }

  dup2(out_fd, 1);
  close(out_fd);

  // Redirect err
  dup2(default_err, 2);

  pid = fork();
  if (pid == -1) {
    perror("ls_grep: fork");
    exit(2);
  }

  if (pid == 0) {
    //Child

    // close file descriptors that are not needed
    close(fdpipe[0]);
    close(fdpipe[1]);
    close(default_in);
    close(default_out);
    close(default_err);

    // You can use execvp() instead if the arguments are stored in an array
    execlp(g_grep, g_grep, argv[2], NULL);

    // exec() is not suppose to return, something went wrong
    perror("ls_grep: exec grep");
    exit(2);

  } else {
    // Parent

    // Restore input, output, and error

    dup2(default_in, 0);
    dup2(default_out, 1);
    dup2(default_err, 2);

    // Close file descriptors that are not needed
    close(fdpipe[0]);
    close(fdpipe[1]);
    close(default_in);
    close(default_out);
    close(default_err);

    // Wait for last process in the pipe line
    waitpid(pid, 0, 0);
  }

  exit(0);
} /* main() */
