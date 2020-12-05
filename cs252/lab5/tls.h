#ifndef TLS_H
#define TLS_H

#include <arpa/inet.h>
#include <openssl/ssl.h>

typedef struct {
  int socket_fd;
  struct sockaddr_in addr;
  SSL *ssl;
} tls_socket;

int close_tls_socket(tls_socket *socket);
int tls_write(tls_socket *socket, char *buf, size_t buf_len);
int tls_read(tls_socket *socket, char *buf, size_t buf_len);

typedef struct {
  int master_socket;
  struct sockaddr_in addr;
  SSL_CTX *ssl_ctx;
} tls_acceptor;

tls_acceptor *create_tls_acceptor(int port);
tls_socket *accept_tls_connection(tls_acceptor *acceptor);
int close_tls_acceptor(tls_acceptor *acceptor);

#endif // TLS_H
