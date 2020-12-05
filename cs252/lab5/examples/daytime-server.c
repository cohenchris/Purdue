#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>

const char * usage =
"                                                               \n"
"daytime-server:                                                \n"
"                                                               \n"
"Simple server program that shows how to use socket calls       \n"
"in the server side.                                            \n"
"                                                               \n"
"To use it in one window type:                                  \n"
"                                                               \n"
"   daytime-server <port>                                       \n"
"                                                               \n"
"Where 1024 < port < 65536.             \n"
"                                                               \n"
"In another window type:                                       \n"
"                                                               \n"
"   telnet <host> <port>                                        \n"
"                                                               \n"
"where <host> is the name of the machine where daytime-server  \n"
"is running. <port> is the port number you used when you run   \n"
"daytime-server.                                               \n"
"                                                               \n"
"Then type your name and return. You will get a greeting and   \n"
"the time of the day.                                          \n"
"                                                               \n";

#define QUEUELEN 5
#define MAXNAME 5

// Handle time request
void handle_request(int socket_fd);

int main(int argc, char **argv) {
  // Print usage if not enough arguments
  if (argc < 2) {
    fprintf(stderr, "%s", usage);
    exit(-1);
  }
  
  // Get the port from the arguments
  int port = atoi(argv[1]);
  
  // Set the IP address and port for this server
  struct sockaddr_in server_addr = { 0 }; 
  memset(&server_addr, 0, sizeof(server_addr));
  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = INADDR_ANY;
  server_addr.sin_port = htons((u_short) port);
  
  // Allocate a socket
  int server_fd = socket(PF_INET, SOCK_STREAM, 0);
  if (server_fd < 0) {
    perror("socket");
    exit(-1);
  }

  // Set socket options to reuse port. Otherwise we will
  // have to wait about 2 minutes before reusing the same port number
  int optval = 1; 
  int err = setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &optval,
                       sizeof(int));
  if (err) {
    perror("setsockopt");
    exit(-1);
  }
   
  // Bind the socket to the IP address and port
  int error = bind(server_fd, (struct sockaddr *) &server_addr,
                   sizeof(server_addr));
  if (error) {
    perror("bind");
    exit(-1);
  }
  
  // Put socket in listening mode and set the 
  // size of the queue of unprocessed connections
  error = listen(server_fd, QUEUELEN);
  if (error) {
    perror("listen");
    exit(-1);
  }

  while (1) {

    // Accept incoming connections
    struct sockaddr_in client_addr = { 0 };
    socklen_t csize = sizeof(client_addr);
    int client_fd = accept(server_fd, (struct sockaddr *) &client_addr,
                           &csize);

    if (client_fd < 0) {
      perror("accept");
      exit(-1);
    }

    // Process request.
    handle_request(client_fd);

    // Close socket
    close(client_fd);
  }
  
}

void handle_request(int fd) {
  // Buffer used to store the name received from the client
  char name[MAXNAME + 1];
  int n = 0;

  // Send prompt
  const char *prompt = "\nType your name: ";
  write(fd, prompt, strlen(prompt));

  // The client should send <name><cr><lf>
  n = recv(fd, name, MAXNAME, 0);
  
  // Trim <CR> <LF>
  if (name[n-2] == '\015' && name[n-1] == '\012') {
    name[n-2] = '\0';
  }
  else if (name[n-1] == '\015') {
    name[n-1] = '\0';
  }
  else {
    name[n] = '\0';
  }

  printf("name = %s\n", name);

  // Get time of day
  time_t now = { 0 };
  time(&now);
  char *time_str = ctime(&now);

  // Send name and greetings
  const char *hi = "\nHi, ";
  const char *time_is = ", the time is: ";
  write(fd, hi, strlen(hi));
  write(fd, name, strlen(name));
  write(fd, time_is, strlen(time_is));
  
  // Send the time of day 
  write(fd, time_str, strlen(time_str));

  // Send last newline
  write(fd, "\n", 1);
}
