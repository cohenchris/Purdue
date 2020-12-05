#include "misc.h"

#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define INIT_BUF_SIZE (16)
#define DIR_LIST_INITIAL_BUF_SIZE (1024)
#define HEADER_FORMAT "<!doctype html>\n\n" \
    "<title>%s</title>\n" \
    "<h1>%s</h1>\n" \
    "<ul>\n"
#define LIST_ITEM_FORMAT "<li>\n" \
    "<a href=\"%s\">%s</a>\n" \
    "</li>\n"
#define FOOTER_FORMAT "</ul>\n"

/*
 * Returns the Content-Type value that should be used for a given filename. The
 * returned string must be free()'d by the caller.
 */

char *get_content_type(char *filename) {
  // initial size of buffer, reading from pipe

  int pipe_fd[2] = { 0 };
  if (pipe(pipe_fd) == -1) {
    perror("get_content_type pipe error");
    exit(-1);
  }
  int pid = fork();

  if (pid == -1) {
    perror("get_content_type fork error");
    exit(-1);
  }
  if (pid == 0) {
    // close read end

    close(pipe_fd[0]);
    dup2(pipe_fd[1], STDOUT_FILENO);
    dup2(pipe_fd[1], STDOUT_FILENO);
    close(pipe_fd[1]);

    execl("/usr/bin/file", "file", "-biE", filename, NULL);
    perror("get_content_type execl error");
    exit(-1);
  }

  // close write end

  close(pipe_fd[1]);

  int buf_size = INIT_BUF_SIZE;
  char *buf = malloc(sizeof(char) * buf_size);
  int buf_end = 0;
  int num_read = 0;
  while ((num_read = read(pipe_fd[0], &buf[buf_end], INIT_BUF_SIZE)) > 0) {
    buf_end += num_read;
    buf_size += num_read;
    buf = realloc(buf, sizeof(char) * buf_size);
  }

  // close read end

  close(pipe_fd[0]);

  int status;
  if (waitpid(pid, &status, 0) == -1) {
    perror("get_content_type waitpid error");
    exit(-1);
  }
  if (!WIFEXITED(status) || WEXITSTATUS(status) != 0) {
    fprintf(stderr, "get_content_type returned nonzero status for %s\n",
            filename);
    return NULL;
  }

  // this might end up being a bit too big, but not too small.

  char *response = malloc(sizeof(char) * (buf_end + 1));

  // don't copy spaces

  int response_index = 0;
  for (int i = 0; i < buf_end; i++) {
    if(!isspace(buf[i])) {
      response[response_index++] = buf[i];
    }
  }
  response[response_index] = '\0';

  free(buf);
  return response;
} /* get_content_type() */

/*
 * Generate an HTML string which can be used for the message_body of a
 * response. files must have at least num_files files correctly set. The
 * returned string must be free()'d by the caller.
 */

char *generate_dir_listing(char *dir_name, int num_files, file_link *files) {
  int buffer_size = DIR_LIST_INITIAL_BUF_SIZE;
  while (buffer_size < strlen(HEADER_FORMAT) + strlen(dir_name) + 1) {
    buffer_size *= 2;
  }
  char *buffer = malloc(sizeof(char) * buffer_size);
  snprintf(buffer, buffer_size + 1, HEADER_FORMAT, dir_name, dir_name);

  for (int i = 0; i < num_files; i++) {
    int combined_length = strlen(LIST_ITEM_FORMAT)
      + strlen(files[i].display_name)
      + strlen(files[i].url) + 1;

    char *li = malloc(sizeof(char) * combined_length);
    snprintf(li, combined_length, LIST_ITEM_FORMAT,
             files[i].url, files[i].display_name);

    while (buffer_size < strlen(buffer) + strlen(li) + 1) {
      buffer_size *= 2;
      buffer = realloc(buffer, sizeof(char) * buffer_size);
    }

    strncat(buffer, li, buffer_size - strlen(buffer) - 1);

    free(li);
    li = NULL;
  }

  while (buffer_size < strlen(buffer) + strlen(FOOTER_FORMAT) + 1) {
    buffer_size *= 2;
    buffer = realloc(buffer, sizeof(char) * buffer_size);
  }

  strncat(buffer, FOOTER_FORMAT, buffer_size - strlen(buffer) - 1);

  return buffer;
} /* generate_dir_listing()*/
