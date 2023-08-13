#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>


#define handle_error(msg) \
do {                      \
  perror(msg);            \
  exit(EXIT_FAILURE);     \
} while (0);

#define PORT 8080
#define MC_IP "224.0.0.1"
#define MSG_BUF 256

int main() {
  int sockfd;
  struct sockaddr_in multicast_addr;
  socklen_t size_mcaddr = sizeof(multicast_addr);
  struct ip_mreqn group;
  char msg[MSG_BUF];

  multicast_addr.sin_family = AF_INET;
  multicast_addr.sin_port = htons(PORT);
  multicast_addr.sin_addr.s_addr = inet_addr(MC_IP);

  group.imr_multiaddr.s_addr = inet_addr(MC_IP);
  group.imr_address.s_addr = INADDR_ANY;
  group.imr_ifindex = 0;

  sockfd = socket(AF_INET, SOCK_DGRAM, 0);
  if (sockfd == -1) 
    handle_error("socket");

  if (bind(sockfd, (struct sockaddr *)&multicast_addr, size_mcaddr) == -1)
    handle_error("bind");

  if (setsockopt(sockfd, IPPROTO_IP, IP_ADD_MEMBERSHIP, &group, sizeof(group)) == -1)
    handle_error("setsockopt");

  if (recvfrom(sockfd, msg, MSG_BUF, 0, (struct sockaddr *)&multicast_addr, &size_mcaddr) == -1)
    handle_error("recvfrom");

  printf("msg from multicast addr : %s\n", msg);

  close(sockfd);

  exit(EXIT_SUCCESS);
}