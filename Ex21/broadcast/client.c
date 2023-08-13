#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define handle_error(msg) \
  do {                    \
    perror(msg);          \
    exit(EXIT_FAILURE);   \
  } while (0);

#define MSG_BUF 256
#define BROADCAST_IP "127.0.0.255"
#define BROADCAST_PORT 9999

int main() {
  int sockfd;
  struct sockaddr_in broadcast_addr;
  socklen_t broadcast_size = sizeof(broadcast_addr);
  char msg[MSG_BUF];

  memset(msg, '\0', MSG_BUF);

  broadcast_addr.sin_family = AF_INET;
  broadcast_addr.sin_port = htons(BROADCAST_PORT);
  broadcast_addr.sin_addr.s_addr = inet_addr(BROADCAST_IP);

  sockfd = socket(AF_INET, SOCK_DGRAM, 0);
  if (sockfd == -1) {
    handle_error("socket");
  }

  if (bind(sockfd, (struct sockaddr *)&broadcast_addr, broadcast_size) == -1) {
    handle_error("bind");
  }

  if (recvfrom(sockfd, msg, MSG_BUF, 0, (struct sockaddr *)&broadcast_addr,
               &broadcast_size) == -1) {
    handle_error("recvfrom");
  }
  printf("msg from broadcast %s\n", msg);

  close(sockfd);

  exit(EXIT_SUCCESS);
}
