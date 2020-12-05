#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "http_messages.h"

#define MAX_FILE_PATH (4096)

/*
 * Handle a CGI request
 */

http_response handle_cgi_bin(const http_request *request) {
  http_response resp = { 0 };
  resp.http_version = request->http_version;

  char * exec_path = malloc(sizeof(char) * MAX_FILE_PATH);
  exec_path = strcpy(exec_path, "./http-root-dir");
  exec_path = strcat(exec_path, request->request_uri);

  char ** cgi_args = malloc(2 * sizeof(char *));
  cgi_args[0] = strdup(exec_path);
  cgi_args[1] = NULL;

  execv(exec_path, cgi_args);

  return resp;
} /* handle_cgi_bin() */
