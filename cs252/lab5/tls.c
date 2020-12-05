#include "tls.h"

#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <openssl/bio.h>
#include <openssl/ssl.h>
#include <openssl/err.h>

/*
 * Initializes SSL for use for the server
 */

void initialize_ssl() {
  SSL_load_error_strings();
  OpenSSL_add_ssl_algorithms();
} /* initialize_ssl() */

/*
 * Creates and configures an SSL context.
 */

SSL_CTX * handle_ssl_context() {
  const SSL_METHOD * method = TLS_server_method();
  SSL_CTX * ctx = SSL_CTX_new(method);
  if (!ctx) {
    perror("Unable to create SSL context");
    ERR_print_errors_fp(stderr);
    exit(EXIT_FAILURE);
  }

  SSL_CTX_set_ecdh_auto(ctx, 1);

  // Load certificate into context

  if (SSL_CTX_use_certificate_file(ctx, "cert.pem",
      SSL_FILETYPE_PEM) <= 0) {
    ERR_print_errors_fp(stderr);
    exit(EXIT_FAILURE);
  }

  // Add the private key into context

  if (SSL_CTX_use_PrivateKey_file(ctx, "key.pem", SSL_FILETYPE_PEM) <= 0 ) {
    ERR_print_errors_fp(stderr);
    exit(EXIT_FAILURE);
  }

  return ctx;
}

/*
 * Close and free a TLS socket created by accept_tls_connection(). Return 0 on
 * success. You should use the polymorphic version of this function, which is
 * close_socket() in socket.c.
 */

int close_tls_socket(tls_socket *socket) {
  printf("Closing TLS socket fd %d", socket->socket_fd);

  char inet_pres[INET_ADDRSTRLEN];
  if (inet_ntop(socket->addr.sin_family,
                &(socket->addr.sin_addr),
                inet_pres,
                INET_ADDRSTRLEN)) {
    printf(" from %s", inet_pres);
  }
  putchar('\n');

  int status = close(socket->socket_fd);

  return status;
} /* close_tls_socket() */

/*
 * Read a buffer of length buf_len from the TLS socket. Return the length of the
 * message on successful completion.
 * You should use the polymorphic version of this function, which is socket_read()
 * in socket.c
 */

int tls_read(tls_socket *socket, char *buf, size_t buf_len) {
  // TODO: Add your code to read from the socket
  if (buf == NULL) {
    return -1;
  }
  else {
    int ssl_read_ret = SSL_read(socket->ssl, buf, buf_len);
    if (ssl_read_ret == -1) {
      printf("Unable to read a character\n");
      return ssl_read_ret;
    }
    return ssl_read_ret;
  }
} /* tls_read() */

/*
 * Write a buffer of length buf_len to the TLS socket. Return 0 on success. You
 * should use the polymorphic version of this function, which is socket_write()
 * in socket.c
 */

int tls_write(tls_socket *socket, char *buf, size_t buf_len) {
  if (buf == NULL) {
    return -1;
  }

  // TODO: Add your code to write to the socket

  ssize_t ssl_write_ret = SSL_write(socket->ssl, buf, buf_len);
  if (ssl_write_ret < 0) {
    return ssl_write_ret;
  }
  else if (ssl_write_ret != buf_len) {
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
            buf_in_hex, buf_len, ssl_write_ret);
    return -1;
  }

  return 0;
} /* tls_write() */

/*
 * Create a new TLS socket acceptor, listening on the given port. Return NULL on
 * error. You should ues the polymorphic version of this function, which is
 * create_socket_acceptor() in socket.c.
 */

tls_acceptor *create_tls_acceptor(int port) {
  initialize_ssl();

  tls_acceptor *acceptor = malloc(sizeof(tls_acceptor));

  acceptor->addr.sin_family = AF_INET;
  acceptor->addr.sin_port = htons(port);
  acceptor->addr.sin_addr.s_addr = htonl(INADDR_ANY);

  acceptor->ssl_ctx = handle_ssl_context();

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
           (struct sockaddr *) &acceptor->addr,
           sizeof(acceptor->addr)) < 0) {
    fprintf(stderr, "Unable to bind to socket: %s\n", strerror(errno));
  }

  if (listen(acceptor->master_socket, 50) < 0) {
    fprintf(stderr, "Unable to listen to socket: %s\n", strerror(errno));
  }

  return acceptor;
} /* create_tls_acceptor() */

/*
 * Accept a new connection from the TLS socket acceptor. Return NULL on error,
 * and the new TLS socket otherwise. You should use the polymorphic version of
 * this function, which is accept_connection() in socket.c.
 */

tls_socket *accept_tls_connection(tls_acceptor *acceptor) {
  struct sockaddr_in addr = { 0 };
  socklen_t addr_len = sizeof(addr);
  int socket_fd = accept(acceptor->master_socket,
                         (struct sockaddr *) &addr,
                         &addr_len);

  if (socket_fd == -1) {
    fprintf(stderr, "Unable to accept connection: %s\n", strerror(errno));
    return NULL;
  }

  tls_socket * socket = malloc(sizeof(tls_socket));
  socket->socket_fd = socket_fd;
  socket->addr = addr;

  socket->ssl = SSL_new(acceptor->ssl_ctx);
  SSL_set_fd(socket->ssl, socket_fd);

  int ssl_accept_ret = SSL_accept(socket->ssl);
  if (ssl_accept_ret <= 0) {
    fprintf(stderr, "Unable to accept a connection: %s\n", strerror(errno));
    return NULL;
  }

  char inet_pres[INET_ADDRSTRLEN];
  if (inet_ntop(addr.sin_family,
                &(addr.sin_addr),
                inet_pres,
                INET_ADDRSTRLEN)) {
    printf("Received a connection from %s\n", inet_pres);
  }

  return socket;
} /* accept_tls_connection() */

/*
 * Close and free the passed TLS socket acceptor. Return 0 on success. You
 * should use the polymorphic version of this function, which is
 * close_socket_acceptor() in socket.c.
 */

int close_tls_acceptor(tls_acceptor *acceptor) {
  printf("Closing socket %d\n", acceptor->master_socket);
  int status = close(acceptor->master_socket);
  SSL_CTX_free(acceptor->ssl_ctx);
  free(acceptor);
  acceptor = NULL;
  return status;
} /* close_tls_acceptor() */
