#ifndef TCP_H
#define TCP_H

#include <arpa/inet.h>

typedef struct {
  int socket_fd;
  struct sockaddr_in addr;
} tcp_socket;

int close_tcp_socket(tcp_socket *socket);
int tcp_write(tcp_socket *socket, char *buf, size_t buf_len);
int tcp_read(tcp_socket *socket, char *buf, size_t buf_len);

typedef struct {
  int master_socket;
  struct sockaddr_in addr;
} tcp_acceptor;

tcp_acceptor *create_tcp_acceptor(int port);
tcp_socket *accept_tcp_connection(tcp_acceptor *acceptor);
int close_tcp_acceptor(tcp_acceptor *acceptor);

#endif // TCP_H
