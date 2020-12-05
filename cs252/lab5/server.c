#include "server.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdbool.h>
#include <pthread.h>
#include <wait.h>

#include "http_messages.h"

#define MAX_FILE_PATH (4096)
#define MAX_INT_LENGTH (4096)

char g_user_pass[MAX_USR_PWD_LEN];
pthread_mutex_t g_socket_mutex = { 0 };
pthread_mutex_t g_base64_mutex = { 0 };

extern http_response handle_cgi_bin(const http_request * request);

/*
 * Return a string in a format <user>:<password>
 * either from auth.txt or from your implememtation.
 */

char *return_user_pwd_string(void) {
  // Read from ./auth.txt. Don't change this. We will use it for testing

  FILE *fp = NULL;
  char *line = NULL;
  size_t len = 0;
  fp = fopen("./auth.txt", "r");
  if (fp == NULL) {
    perror("couldn't read auth.txt");
    exit(-1);
  }
  if (getline(&line, &len, fp) == -1) {
    perror("couldn't read auth.txt");
    free(line);
    line = NULL;
    exit(-1);
  }
  sprintf(g_user_pass, "%s", line);
  free(line);
  line = NULL;
  fclose(fp);
  return g_user_pass;
} /* return_user_pwd_string() */

http_request read_socket_buffer(socket_t * sock, char * socket_buffer) {
  http_request request = { 0 };

  /* Finds first space in the buffer, then increment the pointer to
   * socket_buffer to after the first space
   */

  char * first_space = strchr(socket_buffer, ' ');
  request.method = strndup(socket_buffer, first_space - socket_buffer);
  socket_buffer += first_space - socket_buffer + 1;

  /* Read in URL, then increment the pointer to socket_buffer to after the
   * second space
   */

  char * second_space = strchr(socket_buffer, ' ');
  request.request_uri = strndup(socket_buffer, second_space - socket_buffer);
  request.query = strchr(request.request_uri, '?');
  if (request.query == NULL) {
    request.query = "";
  }
  else {
    /* Increments pointer so that question mark isn't included in query */

    request.query++;
    request.request_uri = strndup(request.request_uri, strlen(request.request_uri)
                                  - strlen(request.query) - 1);
  }

  socket_buffer += second_space - socket_buffer + 1;

  /* Read in HTTP protocol version, then increment the pointer to socket_buffer
   * to after the third space
   */

  char * end_string = "\r\n";
  char * end_of_first_line = strstr(socket_buffer, end_string);
  request.http_version = strndup(socket_buffer,
                                 end_of_first_line - socket_buffer);
  socket_buffer += end_of_first_line - socket_buffer;

  // Read in headers until <crlf><crlf>

  char * header_separation_str = ": ";
  for (;;) {
    socket_buffer += strlen(end_string);
    char * next_end_string = strstr(socket_buffer, end_string);
    if (next_end_string == socket_buffer) {
      break;
    }
    else {
      char * header_string = strndup(socket_buffer, next_end_string - socket_buffer);
      char * header_space_ptr = strstr(header_string, header_separation_str);
      header new_header = { 0 };

      /* Key for header using pointer arithmetic */

      new_header.key = strndup(header_string, header_space_ptr - header_string);

      /* Value for header using pointer arithmetic */

      char * val_pointer = header_space_ptr + strlen(header_separation_str);
      new_header.value = strndup(val_pointer,
                                 next_end_string - val_pointer);

      /* Insert new_header into request.headers array */

      request.headers = realloc(request.headers,
                                sizeof(header) * (request.num_headers + 1));
      request.headers[request.num_headers] = new_header;
      request.num_headers++;

      /* Increment socket_buffer to next line, where there is either a header or
       * another <crlf>
       */

      socket_buffer += strlen(end_string) + strlen(new_header.key) +
                       strlen(new_header.value);
    }
  }
  return request;
} /* read_socket_buffer() */

/*
 * Accept connections one at a time and handle them.
 */

void run_linear_server(acceptor *acceptor) {
  pthread_mutex_init(&g_base64_mutex, NULL);
  while (1) {
    socket_t *sock = accept_connection(acceptor);
    handle(sock);
  }
} /* run_linear_server() */

/*
 * Accept connections, creating a different child process to handle each one.
 */

void run_forking_server(acceptor *acceptor) {
  pthread_mutex_init(&g_base64_mutex, NULL);
  while (1) {
    socket_t * sock = accept_connection(acceptor);

    int fork_return = fork();
    if (fork_return == 0) {
      /* If fork() is successful, 0 is returned in the child, so we are currently
       * in the child process and can execute server response as normal.
       */

      handle(sock);
      exit(EXIT_SUCCESS);
    }
    waitpid(fork_return, NULL, 0);
    close_socket(sock);
  }
} /* run_forking_server() */

/*
 * Accept connections, creating a new thread to handle each one.
 */

void run_threaded_server(acceptor *acceptor) {
  pthread_mutex_init(&g_base64_mutex, NULL);
  while (1) {
    socket_t * sock = accept_connection(acceptor);

    pthread_attr_t attr = { 0 };
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

    pthread_t new_thread = { 0 };
    pthread_create(&new_thread, &attr, (void * (*)(void *)) handle, (void *)
                   sock);
  }
} /* run_threaded_server() */

/*
 * Function for each individual thread pool worker. Accepts a connection for
 * each worker, then handles as normal.
 */

void * loop_thread(acceptor * acceptor) {
  while (1) {
    pthread_mutex_lock(&g_socket_mutex);
    socket_t * sock = accept_connection(acceptor);
    pthread_mutex_unlock(&g_socket_mutex);

    handle(sock);
  }
} /* loop_thread() */

/*
 * Accept connections, drawing from a thread pool with num_threads to handle the
 * connections.
 */

void run_thread_pool_server(acceptor *acceptor, int num_threads) {
  pthread_mutex_init(&g_base64_mutex, NULL);
  pthread_mutex_init(&g_socket_mutex, NULL);
  pthread_t * thread_pool = malloc((num_threads) * sizeof(pthread_t));

  for (int i = 0; i < num_threads; i++) {
    pthread_create(&thread_pool[i], NULL,
                   (void * (*)(void *)) loop_thread, (void *) acceptor);
  }
  loop_thread(acceptor);

  //pthread_mutex_destroy(&g_socket_mutex);
} /* run_thread_pool_server() */

/*
 * returns the type of document that 'filepath' is, based on the suffix.
 * Choices:
 * text/plain  text/html  text/css  image/gif
 * image/png  image/jpeg  image/svg+xml
 */

char * get_document_type(char * filepath) {
  if (strstr(filepath, ".html") != 0) {
    return "text/html";
  }
  else if (strstr(filepath, ".css") != 0) {
    return "text/css";
  }
  else if (strstr(filepath, ".gif") != 0) {
    return "image/gif";
  }
  else if (strstr(filepath, ".png") != 0) {
    return "image/png";
  }
  else if (strstr(filepath, ".jpg") != 0) {
    return "image/jpeg";
  }
  else if (strstr(filepath, ".svg") != 0) {
    return "image/svg+xml";
  }
  else {
    return "text/plain";
  }
} /* get_document_type() */

/*
 * Based on the current status code, set response's message body to explain the
 * error code to the client.
 */

char * set_error_message(int current_status_code) {
  char * err_msg = NULL;
  switch(current_status_code) {
    case 400:
      err_msg = "Bad Request\n";
      break;
    case 401:
      err_msg = "Unauthorized\n";
      break;
    case 403:
      err_msg = "Forbidden\n";
      break;
    case 404:
      err_msg = "Not Found\n";
      break;
    case 405:
      err_msg = "Method Not Allowed\n";
      break;
    case 500:
      err_msg = "Internal Server Error\n";
      break;
    case 505:
      err_msg = "HTTP Version Not Supported\n";
      break;
    default:
      err_msg = "Unhandled HTTP Exception\n";
      break;
  }
  return err_msg;
} /* set_error_message() */

/*
 * Handle an incoming connection on the passed socket.
 */

void handle(socket_t *sock) {
  int current_status_code = 200;

  char * socket_buffer = NULL;
  int buffer_size = 0;

  char read_char = '\0';
  char prev_char = '\0';
  char prev_prev_char = '\0';
  char prev_prev_prev_char = '\0';

  /* Loop to read the entire request string. Assumes <crlf><crlf> denotes the
   * end of a request
   */

  int socket_read_return = -1;

  for (;;) {
    /* read everything from socket buffer */

    read_char = '\0';
    socket_read_return = socket_read(sock, &read_char, sizeof(read_char));

    if (socket_read_return < 0) {
      perror("socket read");
      exit(EXIT_FAILURE);
    }

    buffer_size++;
    socket_buffer = realloc(socket_buffer, sizeof(char) * (buffer_size + 1));
    socket_buffer[buffer_size - 1] = read_char;
    socket_buffer[buffer_size] = '\0';

    if ((read_char == '\n') && (prev_char == '\r') && (prev_prev_char == '\n')
        && (prev_prev_prev_char == '\r')) {
      // If '\r\n\r\n' occurs, end of HTTP request (<crlf><crlf>)

      socket_buffer = realloc(socket_buffer, sizeof(char) * (buffer_size + 1));
      socket_buffer[buffer_size] = '\0';
      break;
    }

    if ((read_char == '\n') && (prev_char != '\r')) {
      current_status_code = 400;
      break;
    }

    // Keeps track of previous 4 characters in order to look for '\r\n\r\n'

    prev_prev_prev_char = prev_prev_char;
    prev_prev_char = prev_char;
    prev_char = read_char;
  }

  /* Check for 400 err (malformed request). 400 err if there are fewer than 2
   * spaces.
   */

  char * end_first_line = strstr(socket_buffer, "\r\n");
  char * first_line = strndup(socket_buffer, end_first_line - socket_buffer);
  char * first_space = strchr(first_line, ' ');
  if (first_space == NULL) {
    current_status_code = 400;
  }
  else {
    first_line = first_space + 1;
    char * second_space = strchr(first_line, ' ');
    if (second_space == NULL) {
      current_status_code = 400;
    }
  }

  http_request request = { 0 };
  if (current_status_code == 200) {
    request = read_socket_buffer(sock, socket_buffer);
  }
  request.message_body = "";
  print_request(&request);

  http_response response = { 0 };

  /* BASIC HTTP AUTHENTICATION
   * check if 'Authorization' is a header. If not, 401 error, and respond with
   * header 'WWW-Authenticate: Basic realm = "Cohen"'
   */

  if ((request.num_headers == 0) && (current_status_code == 200)) {
    current_status_code = 401;
  }


  for (int i = 0; i < request.num_headers; i++) {
    if (!strcmp(request.headers[i].key, "Authorization")) {
      return_user_pwd_string();
      char * request_auth_pass = strchr(request.headers[i].value, ' ');
      if (request_auth_pass == NULL) {
        break;
      }
      request_auth_pass++;

      pthread_mutex_lock(&g_base64_mutex);
      system("cat ./auth.txt | base64 > base64_creds.txt");
      pthread_mutex_unlock(&g_base64_mutex);

      FILE *fp = NULL;
      char *line = (char *) malloc(sizeof(char) * MAX_USR_PWD_LEN);
      size_t len = 0;

      fp = fopen("./base64_creds.txt", "r");
      if (fp == NULL) {
        perror("couldn't read base64_creds.txt");
        exit(-1);
      }

      if (getline(&line, &len, fp) == -1) {
        fclose(fp);
        fp = NULL;
        perror("couldn't read base64_creds.txt");
        exit(-1);
      }

      char * base64_user_pass = malloc(sizeof(char) * MAX_USR_PWD_LEN);
      sprintf(base64_user_pass, "%s", line);

      // Remove padding from base64 bash output

      char * new_base64_user_pass = strndup(base64_user_pass,
                                            strlen(base64_user_pass) - 5);

      if (strcmp(request_auth_pass, new_base64_user_pass)) {
        current_status_code = 401;
      }

      fclose(fp);
      fp = NULL;
      break;
    }

    if (i == request.num_headers - 1) {
      current_status_code = 401;
    }
  }

  if (current_status_code != 400) {
    if (strcmp(request.method, "GET")) {
      /* Server does not handle anything other than 'GET' right now. */

      current_status_code = 405;
    }

    response.http_version = request.http_version;
    if ((strcmp(response.http_version, "HTTP/1.1") &&
        (strcmp(response.http_version, "HTTP/1.0")))) {
      current_status_code = 505;
    }

  }

  /* File path starts at ./http-root-dir/htdocs. If the URI is just '/', we send
   * the index.html file in ./http-root-dir, so append that. Otherwise, the file
   * path needed is appended in the line preceding the if-statement.
   * If the file is a cgi-bin executable, it will just start at ./http-root-dir.
   */

  response.message_body = "";

  if (current_status_code == 200) {
    char * file_path = malloc(sizeof(char) * MAX_FILE_PATH);
    if (strstr(request.request_uri, "cgi-bin") != NULL) {
      // file path is going to be different if it's a cgi bin request

      file_path = strcpy(file_path, "./http-root-dir");
    }
    else {
      file_path = strcpy(file_path, "./http-root-dir/htdocs");
    }

    strcat(file_path, request.request_uri);

    if (!strcmp(request.request_uri, "/")) {
      strcat(file_path, "index.html");
    }

    struct stat file_stats = { 0 };
    stat(file_path, &file_stats);

    if (file_stats.st_mode & S_IFDIR) {
      /* If the request uri is a directory, the new desired file path is
       * <dir>/index.html
       */

      char * index_html = "/index.html";
      char * index_html_path = strdup(file_path);
      index_html_path = realloc(index_html_path, strlen(index_html_path) +
                                strlen(index_html) + 1);
      strcat(index_html_path, index_html);

      file_path = realloc(file_path, sizeof(char) * (strlen(file_path) +
                                                     strlen(index_html) + 1));
      strcat(file_path, index_html);
    }

    if ((access(file_path, F_OK) == 0) && (access(file_path, R_OK) != 0)) {
      current_status_code = 403;
    }

    else if ((access(file_path, F_OK) == 0) && (current_status_code == 200)) {
      response.num_headers++;
      response.headers = realloc(response.headers,
                                 sizeof(header) * response.num_headers);
      header connection_header = { 0 };
      connection_header.key = strdup("Connection");
      connection_header.value = strdup("close");
      response.headers[response.num_headers - 1] = connection_header;

      response.num_headers++;
      response.headers = realloc(response.headers,
                                 sizeof(header) * response.num_headers);
      header content_type_header = { 0 };
      content_type_header.key = strdup("Content-Type");
      content_type_header.value = strdup(get_document_type(file_path));
      response.headers[response.num_headers - 1] = content_type_header;

      response.num_headers++;
      response.headers = realloc(response.headers,
                                 sizeof(header) * response.num_headers);
      header content_length_header = { 0 };
      content_length_header.key = strdup("Content-Length");
      FILE * file_fp = fopen(file_path, "r");
      fseek(file_fp, 0, SEEK_END);
      int file_size = ftell(file_fp);
      rewind(file_fp);
      fclose(file_fp);
      file_fp = NULL;
      char * file_size_str = malloc(sizeof(char) * MAX_INT_LENGTH);
      sprintf(file_size_str, "%d", file_size);
      content_length_header.value = file_size_str;
      response.headers[response.num_headers - 1] = content_length_header;

      for (int i = 0; i < request.num_headers; i++) {
        response.num_headers++;
        response.headers = realloc(response.headers,
                                   sizeof(header) * response.num_headers);
        response.headers[response.num_headers - 1].key = strdup(request
                                                                .headers[i]
                                                                .key);
        response.headers[response.num_headers - 1].value = strdup(request
                                                                  .headers[i]
                                                                  .value);
      }

      /* Write requested data to the socket. */

      FILE * data_file = NULL;
      bool is_image = false;

      if ((strstr(content_type_header.value, "image") != NULL) &&
          (strstr(content_type_header.value, "svg") == NULL)) {
        /* Have to read an image in binary mode
         * Excludes svg files since they can be read normally
         */

        is_image = true;
        data_file = fopen(file_path, "rb");
      }
      else {
        /* Any text files, if read in binary mode, will not display how we want
         * them to, so read them in normal mode.
         */

        data_file = fopen(file_path, "r");
      }

      char * data = NULL;

      if (is_image) {
        char ch = '\0';
        data = malloc(sizeof(char) * (file_size + 1));
        int read_return = -1;
        int i = 0;
        while ((read_return = read(fileno(data_file), &ch, sizeof(ch))) == 1) {
          data[i++] = ch;
        }
      }
      else {
        data = malloc(sizeof(char) * file_size + 1);
        int fread_return = fread(data, 1, file_size, data_file);
        if (fread_return <= 0) {
          perror("fread");
          exit(EXIT_FAILURE);
        }
      }

      if (data == NULL) {
        response.message_body = "";
      }
      else {
        response.message_body = data;
      }
      response.msg_body_len = file_size;

      fclose(data_file);
      data_file = NULL;
    }
    else if ((access(file_path, F_OK) != 0) && (current_status_code == 200)) {
      current_status_code = 404;
    }

    free(file_path);
    file_path = NULL;
  }

  /* current_status_code will be 200 (OK) if nothing went wrong. */

  if (current_status_code != 200) {
    for (int i = 0; i < response.num_headers; i++) {
      /* Delete all headers that exist, since an error message only requires the
       * Content-Type header.
       */

      free(response.headers[i].key);
      response.headers[i].key = NULL;
      free(response.headers[i].value);
      response.headers[i].value = NULL;
    }

    response.num_headers = 1;

    if (current_status_code == 401) {
      response.headers = realloc(response.headers,
                                 sizeof(header) * response.num_headers);
      header authorization_header = { 0 };
      authorization_header.key = strdup("WWW-Authenticate");
      authorization_header.value = strdup("Basic Realm=\"Cohen\"");
      response.headers[response.num_headers - 1] = authorization_header;
    }
    else {
      response.headers = realloc(response.headers,
                                 sizeof(header) * response.num_headers);
      header content_type_header = { 0 };
      content_type_header.key = strdup("Content-Type");
      content_type_header.value = strdup("text/plain");

      response.headers[response.num_headers - 1] = content_type_header;
      response.message_body = set_error_message(current_status_code);
      response.http_version = "HTTP/1.1";
    }
  }
  else {
    if (strstr(request.request_uri, "cgi-bin") != NULL) {
      // cgi-bin request

      int pipe_fds[2];
      if (pipe(pipe_fds) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
      }
      int input_fd = pipe_fds[0];
      int output_fd = pipe_fds[1];

      int fork_return = fork();
      if (fork_return == 0) {
        // child process

        if (strlen(request.query) != 0) {
          // if query is not empty, set request method to GET

          setenv("REQUEST_METHOD", "GET", 1);
        }
        setenv("QUERY_STRING", request.query, 1);

        /* REDIRECT OUTPUT TO SLAVE SOCKET */

        if (dup2(output_fd, 1) == -1) {
          perror("dup2");
          exit(EXIT_FAILURE);
        }

        char * exec_path = malloc(sizeof(char) * MAX_FILE_PATH);
        exec_path = strcpy(exec_path, "./http-root-dir");
        exec_path = strcat(exec_path, request.request_uri);

        /* 2 arguments every time because the first argument is the executable
         * (similar to $0 in bash), and execv needs null-terminated arg list.
         */

        char ** cgi_args = malloc(2 * sizeof(char *));
        cgi_args[0] = strdup(exec_path);
        cgi_args[1] = NULL;

        execv(exec_path, cgi_args);
        exit(EXIT_FAILURE);
      }

      close(output_fd);
      waitpid(fork_return, NULL, 0);

      char response_char = '\0';
      char prev_response_char = '\0';
      char prev_prev_response_char = '\0';
      char prev_prev_prev_response_char = '\0';
      char response_buffer[1000];
      int buffer_index = 0;
      bool removed_header = false;

      while (read(input_fd, &response_char, sizeof(char)) != 0) {
        response_buffer[buffer_index] = response_char;
        buffer_index++;

        if ((response_char == '\n') && (prev_response_char == '\r') &&
            (prev_prev_response_char == '\n') &&
            (prev_prev_prev_response_char == '\r')) {
          if (removed_header == false) {
            for (int i = 0; i < response.num_headers; i++) {
              if (!strcmp(response.headers[i].key, "Content-Type")) {
                // replace Content-Type to type that executable outputted

                response.headers[i].value = strndup(strstr(response_buffer, ": ")
                                                    + 2, buffer_index - 18);
                memset(&response_buffer, 0, sizeof(response_buffer));
                buffer_index = 0;
                removed_header = true;
                break;
              }
            }
          }
        }
        else if ((response_char == '\n') && (prev_response_char == '\n')) {
          if (removed_header == false) {
            for (int i = 0; i < response.num_headers; i++) {
              if (!strcmp(response.headers[i].key, "Content-Type")) {
                // replace Content-Type to type that executable outputted

                response.headers[i].value = strndup(strstr(response_buffer, ": ")
                                                    + 2, buffer_index - 16);
                memset(&response_buffer, 0, sizeof(response_buffer));
                buffer_index = 0;
                removed_header = true;
                break;
              }
            }
          }
        }
        prev_prev_prev_response_char = prev_prev_response_char;
        prev_prev_response_char = prev_response_char;
        prev_response_char = response_char;
      }

      close(input_fd);

      response.message_body = strdup(response_buffer);
      response.msg_body_len = strlen(response_buffer);
      for (int i = 0; i < response.num_headers; i++) {
        if (!strcmp(response.headers[i].key, "Content-Length")) {
          // reset Content-Length to the size of the msg_body_len instead of the
          // cgi bin file

          response.headers[i].value = malloc(sizeof(char) *
                                             response.msg_body_len);
          sprintf(response.headers[i].value, "%d", response.msg_body_len);
        }
      }
    }
  }

  response.status_code = current_status_code;
  response.reason_phrase = (char *) (status_reason(current_status_code));

  char *to_string = response_string(&response);
  printf("%s\n", to_string);
  printf("%s\n", response.message_body);

  socket_write_string(sock, to_string);
  socket_write(sock, response.message_body, response.msg_body_len);

  // Freeing memory

  free(to_string);
  to_string = NULL;
  free(socket_buffer);
  socket_buffer = NULL;
  if ((response.status_code == 200) && (response.message_body != NULL)) {
    free(response.message_body);
    response.message_body = NULL;
  }

  /* Free response and request stuff */
  if (request.method != NULL) {
    free(request.method);
    request.method = NULL;
  }
  if (request.request_uri != NULL) {
    free(request.request_uri);
    request.request_uri = NULL;
  }
  if (request.http_version != NULL) {
    free(request.http_version);
    request.http_version = NULL;
  }
  if (request.num_headers > 0) {
    for (int i = 0; i < request.num_headers; i++) {
      free(request.headers[i].key);
      request.headers[i].key = NULL;
      free(request.headers[i].value);
      request.headers[i].value = NULL;
    }
  }

  close_socket(sock);
} /* handle() */
