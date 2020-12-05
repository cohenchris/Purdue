#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <string.h>

#define BOUND_PORTNO 4433

/*
 * Create a passive socket and bind it to the specified port
 */

int create_socket(int port) {
  int socket_fd = 0;
  struct sockaddr_in addr = { 0 };

  // Set the IP address and port for this server
  addr.sin_family = AF_INET;
  addr.sin_port = htons(port);
  addr.sin_addr.s_addr = htonl(INADDR_ANY);

  // Allocate a socket
  socket_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (socket_fd < 0) {
    perror("Unable to create socket");
    exit(EXIT_FAILURE);
  }

  // Bind the socket to the IP address and port
  if (bind(socket_fd, (struct sockaddr *) &addr, sizeof(addr)) < 0) {
    perror("Unable to bind");
    exit(EXIT_FAILURE);
  }

  // Put socket in listening mode and set the 
  // size of the queue of unprocessed connections
  if (listen(socket_fd, 1) < 0) {
    perror("Unable to listen");
    exit(EXIT_FAILURE);
  }

  return socket_fd;
}

/*
 * Call needed SSL init routines
 */

void init_openssl() { 
  SSL_load_error_strings();	
  OpenSSL_add_ssl_algorithms();
}

/*
 * Handle SSL cleanup (for termination)
 */

/*
 * Create an SSL context
 */

SSL_CTX *create_context() {
  // Method used to create context
  const SSL_METHOD *method;
  SSL_CTX *ctx;

  // Get SSL method, this is a version-flexible method and supports mutiple protocols on the server's side
  method = TLS_server_method();

  // New a context with method created above
  ctx = SSL_CTX_new(method);
  if (!ctx) {
    perror("Unable to create SSL context");
    ERR_print_errors_fp(stderr);
    exit(EXIT_FAILURE);
  }

  return ctx;
}

/*
 * Configure a context by specifying the certificate and private key
 */

void configure_context(SSL_CTX *ctx) {
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
}

/*
 * This program listens for an SSL connection on the defined port
 * and replies with a simple hello world response.
 */

int main(int argc, char **argv) {
  // Listening socket and context used in openssl
  int sock = 0;
  SSL_CTX *ctx = NULL;

  // Initialization required by openssl
  init_openssl();

  // Create a context for using openssl
  ctx = create_context();

  // Use key and cert to configure context
  configure_context(ctx);

  // Create a socket with BOUND_PORTNO
  sock = create_socket(BOUND_PORTNO);

  /* Handle connections */
  while (1) {
    struct sockaddr_in addr = { 0 };
    uint len = sizeof(addr);
    SSL *ssl = NULL;
    const char reply[] = "HTTP/1.1 200 OK\r\nConnection: close\r\nContent-Type: text/html\r\nContent-Length: 11\r\n\r\nHello World";
    char request[4096] = { 0 };

    // Accept incoming connections
    int client = accept(sock, (struct sockaddr *) &addr, &len);
    if (client < 0) {
      perror("Unable to accept");
      exit(EXIT_FAILURE);
    }

    // Set the connected socket with ssl context
    ssl = SSL_new(ctx);
    SSL_set_fd(ssl, client);

    if (SSL_accept(ssl) <= 0) {
      ERR_print_errors_fp(stderr);
    }
    else {
      SSL_read(ssl, request, 4096);
      puts(request);
      SSL_write(ssl, reply, strlen(reply));
    }

    // Remember to free the ssl when finished
    SSL_free(ssl);

    // Close socket
    close(client);
  }

  // Close the listening socket
  close(sock);

  // Free the ssl context
  SSL_CTX_free(ctx);
}


