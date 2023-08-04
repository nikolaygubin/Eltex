#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define IP "127.0.0.2"
#define PORT 8080
#define BUF_SIZE 256

#define handle_error(msg) \
  do {                    \
    perror(msg);          \
    exit(EXIT_FAILURE);   \
  } while (0);

int main() {
  int sockfd;
  char buf[BUF_SIZE];
  struct sockaddr_in server_addr, client_addr;
  socklen_t server_addr_size = sizeof(server_addr),
            client_addr_size = sizeof(client_addr);

  sockfd = socket(AF_INET, SOCK_DGRAM, 0);
  if (sockfd == -1) handle_error("socket");

  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(PORT);
  server_addr.sin_addr.s_addr = inet_addr(IP);

  if (bind(sockfd, (struct sockaddr *)&server_addr, server_addr_size) == -1)
    handle_error("bind");


  if (recvfrom(sockfd, buf, BUF_SIZE, 0, (struct sockaddr *)&client_addr,
               &client_addr_size) == -1)
    handle_error("recvfrom");

  printf("message from client : %s\n", buf);

  if (sendto(sockfd, "msg recive!", BUF_SIZE, 0,
             (struct sockaddr *)&client_addr, client_addr_size) == -1)
    handle_error("sendto");

  if (close(sockfd) == -1) handle_error("close");

  exit(EXIT_SUCCESS);
}