#include "http_messages.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define ERR_CODE_LEN (4)
#define HEADER_SEPARATION_SIZE (2)
#define END_LINE_LEN (2)
#define SPACE (1)

/*
 * Return the reason string for a particular status code. You might find this
 * helpful when implementing response_string().
 */

const char *status_reason(int status) {
  switch (status) {
    case 100:
      return "Continue";
    case 101:
      return "Switching Protocols";
    case 200:
      return "OK";
    case 201:
      return "Created";
    case 202:
      return "Accepted";
    case 203:
      return "Non-Authoritative Information";
    case 204:
      return "No Content";
    case 205:
      return "Reset Content";
    case 206:
      return "Partial Content";
    case 300:
      return "Multiple Choices";
    case 301:
      return "Moved Permanently";
    case 302:
      return "Found";
    case 303:
      return "See Other";
    case 304:
      return "Not Modified";
    case 305:
      return "Use Proxy";
    case 307:
      return  "Temporary Redirect";
    case 400:
      return "Bad Request";
    case 401:
      return "Unauthorized";
    case 402:
      return "Payment Required";
    case 403:
      return "Forbidden";
    case 404:
      return "Not Found";
    case 405:
      return "Method Not Allowed";
    case 406:
      return "Not Acceptable";
    case 407:
      return "Proxy Authentication Required";
    case 408:
      return "Request Time-out";
    case 409:
      return "Conflict";
    case 410:
      return "Gone";
    case 411:
      return "Length Required";
    case 412:
      return "Precondition Failed";
    case 413:
      return "Request Entity Too Large";
    case 414:
      return "Request-URI Too Large";
    case 415:
      return  "Unsupported Media Type";
    case 416:
      return "Requested range not satisfiable";
    case 417:
      return "Expectation Failed";
    case 500:
      return "Internal Server Error";
    case 501:
      return "Not Implemented";
    case 502:
      return "Bad Gateway";
    case 503:
      return "Service Unavailable";
    case 504:
      return "Gateway Time-out";
    case 505:
      return "HTTP Version not supported";
    default:
      return "Unknown status";
  }
} /* status_reason() */

/*
 * Create the actual response string to be sent over the socket, based
 * on the parameter.
 */

char *response_string(http_response *response) {
  /* Adds space for:
   * <http_version> <err_code> <err_code_phrase> <crlf>
   */

  int response_length = strlen(response->http_version) + SPACE + ERR_CODE_LEN +
                        SPACE + strlen(response->reason_phrase) + END_LINE_LEN;

  for (int i = 0; i < response->num_headers; i++) {
    /* Adds space for:
     * <header_key>: <header_value> <crlf>
     */

    response_length += strlen(response->headers[i].key) +
                       HEADER_SEPARATION_SIZE +
                       strlen(response->headers[i].value) +
                       END_LINE_LEN;
  }

  /* Adds space for:
   * <crlf>
   * <data>
   */

  response_length += END_LINE_LEN;

  /* Adds every field to the response string */

  char * to_string = malloc(sizeof(char) * response_length);

  to_string = strcpy(to_string, response->http_version);

  strcat(to_string, " ");

  char * status_code_str = malloc(sizeof(char) * ERR_CODE_LEN);
  sprintf(status_code_str, "%d", response->status_code);
  strcat(to_string, status_code_str);

  strcat(to_string, " ");
  strcat(to_string, response->reason_phrase);
  strcat(to_string, "\r\n");

  for (int i = 0; i < response->num_headers; i++) {
    strcat(to_string, response->headers[i].key);
    strcat(to_string, ": ");
    strcat(to_string, response->headers[i].value);
    strcat(to_string, "\r\n");
  }
  strcat(to_string, "\r\n");

  return to_string;
} /* response_string() */

/*
 * Print the request to stdout, useful for debugging.
 */

void print_request(http_request *request) {
  // Magic string to help with autograder

  printf("\\\\==////REQ\\\\\\\\==////\n");

  printf("Method: {%s}\n", request->method);
  printf("Request URI: {%s}\n", request->request_uri);
  printf("Query string: {%s}\n", request->query);
  printf("HTTP Version: {%s}\n", request->http_version);

  printf("Headers: \n");
  for (int i = 0; i < request->num_headers; i++) {
    printf("field-name: %s; field-value: %s\n",
           request->headers[i].key, request->headers[i].value);
  }

  printf("Message body length: %ld\n", strlen(request->message_body));
  printf("%s\n", request->message_body);

  // Magic string to help with autograder

  printf("//==\\\\\\\\REQ////==\\\\\n");
} /* print_request() */
