#include "tcp.h"

#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * Close and free a TCP socket created by accept_tcp_connection(). Return 0 on
 * success. You should use the polymorphic version of this function, which is
 * close_socket() in socket.c.
 */

int close_tcp_socket(tcp_socket *socket) {
  printf("Closing TCP socket fd %d", socket->socket_fd);

  char inet_pres[INET_ADDRSTRLEN];
  if (inet_ntop(socket->addr.sin_family,
                &(socket->addr.sin_addr),
                inet_pres,
                INET_ADDRSTRLEN)) {
    printf(" from %s", inet_pres);
  }
  putchar('\n');

  int status = close(socket->socket_fd);
  free(socket);

  return status;
} /* close_tcp_socket() */

/*
 * Read a buffer of length buf_len from the TCP socket. Return the length of the
 * message on successful completion.
 * You should use the polymorphic version of this function, which is
 * socket_read() in socket.c
 */

int tcp_read(tcp_socket *socket, char *buf, size_t buf_len) {
  if (buf == NULL){
    return -1;
  }
  else{
    int r = recv(socket->socket_fd, buf, buf_len, 0);
    if (r == -1){
      printf("Unable to read a character\n");
      return r;
    }
    return r;
  }
} /* tcp_read() */

/*
 * Write a buffer of length buf_len to the TCP socket. Return 0 on success. You
 * should use the polymorphic version of this function, which is socket_write()
 * in socket.c
 */

int tcp_write(tcp_socket *socket, char *buf, size_t buf_len) {
  if (buf == NULL) {
    return -1;
  }

  size_t sent = send(socket->socket_fd, buf, buf_len, 0);
  if (sent < 0) {
    return sent;
  }
  else if (sent != buf_len) {
    size_t i = 0;

    // Assume 2 hex characters per character

    char buf_in_hex[(buf_len * 2) + 1];
    for (i = 0; i < buf_len; i++) {
      char current_hex[2 + 1];
      snprintf(current_hex, 2 + 1, "%x", buf[i]);
      strncat(buf_in_hex, current_hex, 2);
    }

    fprintf(stderr,
            "Could not write all bytes of: '%s'. "
            "Expected %ld but actually sent %ld\n",
            buf_in_hex, buf_len, sent);
    return -1;
  }

  return 0;
} /* tcp_write() */

/*
 * Create a new TCP socket acceptor, listening on the given port. Return NULL on
 * error. You should ues the polymorphic version of this function, which is
 * create_socket_acceptor() in socket.c.
 */

tcp_acceptor *create_tcp_acceptor(int port) {
  tcp_acceptor *acceptor = malloc(sizeof(tcp_acceptor));

  acceptor->addr.sin_family = AF_INET;
  acceptor->addr.sin_port = htons(port);
  acceptor->addr.sin_addr.s_addr = htonl(INADDR_ANY);

  acceptor->master_socket = socket(AF_INET, SOCK_STREAM, 0);
  if (acceptor->master_socket < 0) {
    fprintf(stderr, "Unable to create socket: %s\n", strerror(errno));
    return NULL;
  }

  int optval = 1;
  if (setsockopt(acceptor->master_socket,
                 SOL_SOCKET,
                 SO_REUSEADDR,
                 &optval,
                 sizeof(optval)) < 0) {
    fprintf(stderr, "Unable to set socket options: %s\n", strerror(errno));
  }
  if (bind(acceptor->master_socket,
           (struct sockaddr*) &acceptor->addr,
           sizeof(acceptor->addr)) < 0) {
    fprintf(stderr, "Unable to bind to socket: %s\n", strerror(errno));
  }

  if (listen(acceptor->master_socket, 50) < 0) {
    fprintf(stderr, "Unable to listen to socket: %s\n", strerror(errno));
  }

  return acceptor;
} /* create_tcp_acceptor() */

/*
 * Accept a new connection from the TCP socket acceptor. Return NULL on error,
 * and the new TCP socket otherwise. You should use the polymorphic version of
 * this function, which is accept_connection() in socket.c.
 */

tcp_socket *accept_tcp_connection(tcp_acceptor *acceptor) {
  struct sockaddr_in addr = { 0 };
  socklen_t addr_len = sizeof(addr);

  //TODO: SSL_accept?
  int socket_fd = accept(acceptor->master_socket,
                         (struct sockaddr*) &addr,
                         &addr_len);
  if (socket_fd == -1) {
    fprintf(stderr, "Unable to accept connection: %s\n", strerror(errno));
    return NULL;
  }

  tcp_socket *socket = malloc(sizeof(tcp_socket));
  socket->socket_fd = socket_fd;
  socket->addr = addr;

  char inet_pres[INET_ADDRSTRLEN];
  if (inet_ntop(addr.sin_family,
                &(addr.sin_addr),
                inet_pres,
                INET_ADDRSTRLEN)) {
    printf("Received a connection from %s\n", inet_pres);
  }

  return socket;
} /* accept_tcp_connection() */

/*
 * Close and free the passed TCP socket acceptor. Return 0 on success. You
 * should use the polymorphic version of this function, which is
 * close_socket_acceptor() in socket.c.
 */

int close_tcp_acceptor(tcp_acceptor *acceptor) {
  printf("Closing socket %d\n", acceptor->master_socket);
  int status = close(acceptor->master_socket);
  free(acceptor);
  return status;
} /* close_tcp_acceptor() */
