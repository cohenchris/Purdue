#ifndef SOCKET_H
#define SOCKET_H

#ifdef USE_TLS
#include "tls.h"
typedef tls_socket socket_t;
typedef tls_acceptor acceptor;
#else
#include "tcp.h"
typedef tcp_socket socket_t;
typedef tcp_acceptor acceptor;
#endif

int socket_read(socket_t *socket, char *buffer, size_t length);
int socket_write_string(socket_t *socket, char *string);
int socket_write(socket_t *socket, char *buffer, size_t length);
int close_socket(socket_t *socket);

acceptor *create_socket_acceptor(int port);
socket_t *accept_connection(acceptor *socket_acceptor);
int close_socket_acceptor(acceptor* socket_acceptor);

#endif // SOCKET_H
