#include <arpa/inet.h>
#include <netinet/in.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define START_MES "OK"
#define BUF_SIZE 1024
#define handle_error(msg) \
  do {                    \
    perror(msg);          \
    exit(EXIT_FAILURE);   \
  } while (0);
#define PORT 8080
#define MAX_NUM_CLIENTS 1000

int main() {
  int sockfd;
  char buf[BUF_SIZE];
  struct sockaddr_in server;

  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd == -1) handle_error("socket");

  server.sin_family = AF_INET;
  server.sin_port = htons(PORT);
  server.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

  if (connect(sockfd, (struct sockaddr *)&server, sizeof(struct sockaddr_in)) == -1)
      handle_error("connect");

  if (recv(sockfd, buf, BUF_SIZE, 0) == -1) handle_error("recv");
  printf("%s\n", buf);

  if (send(sockfd, START_MES, BUF_SIZE, 0) == -1) handle_error("send");

  close(sockfd);

  exit(EXIT_SUCCESS);
}
