#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>

#define PORT 8080
#define IP "127.0.0.1"
#define BUF_SIZE 256

#define handle_error(msg) \
  do {                    \
    perror(msg);          \
    exit(EXIT_FAILURE);   \
  } while (0);

int main() {
  int sockfd, count = 0;
  char buf[BUF_SIZE];
  struct sockaddr_in server, client;
  socklen_t server_size = sizeof(server), client_size = sizeof(client);

  server.sin_family = AF_INET;
  server.sin_port = htons(PORT);
  server.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
  // server.sin_addr.s_addr = inet_addr(IP);

  sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_UDP);
  if (sockfd == -1) handle_error("socket");

  if (bind(sockfd, (struct sockaddr *) &server, server_size) == -1) handle_error("bind");

  while (1) {
    if (recvfrom(sockfd, buf, BUF_SIZE, 0, (struct sockaddr *)&client,
                 &client_size) == -1)
      handle_error("rcvfrom");
    printf("Get new pocket! Count : %d\n", count++);
  }

  exit(EXIT_SUCCESS);
}