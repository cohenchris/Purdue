#include "socket.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/*
 * Polymorphically read a number of characters specified by length from buffer
 * from socket. Return the length of the message on successful completion.
 */

int socket_read(socket_t *socket, char *buffer, size_t length) {
#ifdef USE_TLS
  return tls_read(socket, buffer, length);
#else
  return tcp_read(socket, buffer, length);
#endif
} /* socket_read() */

/*
 * Write a null-terminated string to the socket. Return 0 on success.
 */

int socket_write_string(socket_t *socket, char *string) {
  return socket_write(socket, string, strlen(string));
} /* socket_write_string() */

/*
 * Polymorphically write a number of characters specified by length from buffer
 * to socket. Return 0 on success.
 */

int socket_write(socket_t *socket, char *buffer, size_t length) {
#ifdef USE_TLS
  return tls_write(socket, buffer, length);
#else
  return tcp_write(socket, buffer, length);
#endif
} /* socket_write() */

/*
 * Polymorphically close and free the socket. Return 0 on success. The caller
 * should set socket to NULL afterwards to avoid referencing a socket which will
 * be invalid.
 */

int close_socket(socket_t *socket) {
#ifdef USE_TLS
  return close_tls_socket(socket);
#else
  return close_tcp_socket(socket);
#endif
} /* close_socket() */

/*
 * Polymorphically create a new socket acceptor, taking the port number as an
 * argument. Return NULL on error. It is the caller's responsibility to call
 * close_socket_acceptor() to close and free the acceptor.
 */

acceptor *create_socket_acceptor(int port) {
#ifdef USE_TLS
  return create_tls_acceptor(port);
#else
  return create_tcp_acceptor(port);
#endif
} /* create_socket_acceptor() */

/*
 * Polymorphically accept a new connection on the passed socket acceptor. Return
 * NULL on error.
 */

socket_t *accept_connection(acceptor *socket_acceptor) {
#ifdef USE_TLS
  return accept_tls_connection(socket_acceptor);
#else
  return accept_tcp_connection(socket_acceptor);
#endif
} /* accept_connection() */

/*
 * Polymorphically close and free the passed socket acceptor. Return 0 on
 * success. The caller should set socket_acceptor to NULL afterwards to avoid
 * referencing an acceptor which will be invalid.
 */

int close_socket_acceptor(acceptor* socket_acceptor) {
#ifdef USE_TLS
  return close_tls_acceptor(socket_acceptor);
#else
  return close_tcp_acceptor(socket_acceptor);
#endif
} /* close_socket_acceptor() */
