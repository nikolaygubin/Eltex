#include <arpa/inet.h>
#include <netinet/ether.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/udp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define IP "127.0.0.1"
#define PORT 8080
#define CLIENT_PORT 7777
#define BUF_SIZE 256
#define START_IDX_PAYLOAD 8

#define handle_error(msg) \
  do {                    \
    perror(msg);          \
    exit(EXIT_FAILURE);   \
  } while (0);

int main() {
  int sockfd;
  char buf[BUF_SIZE], msg[] = "Hello!\0";
  struct sockaddr_in server_addr;
  socklen_t server_addr_len = sizeof(server_addr);
  struct udphdr *header_udp;
  struct iphdr *header_ip;
  int optval = 1;

  sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_UDP);
  if (sockfd == -1) handle_error("socket");

  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(PORT);
  server_addr.sin_addr.s_addr = inet_addr(IP);

  memset(buf, '\0', BUF_SIZE);
  header_udp = (struct udphdr *)(buf + sizeof(struct iphdr));
  header_udp->dest = htons(PORT);
  header_udp->source = htons(CLIENT_PORT);
  header_udp->len = htons(sizeof(struct udphdr) + strlen(msg));
  header_udp->check = 0;

  if (setsockopt(sockfd, IPPROTO_IP, IP_HDRINCL, &optval, sizeof(optval)) == -1)
    handle_error("setsockopt");

  header_ip = (struct iphdr *)buf;
  header_ip->version = 4;
  header_ip->ihl = 5;
  header_ip->tos = 0;
  header_ip->tot_len = sizeof(struct iphdr);
  header_ip->id = 0;
  header_ip->frag_off = 0;
  header_ip->ttl = 255;
  header_ip->protocol = IPPROTO_UDP;
  header_ip->check = 0;
  header_ip->saddr = inet_addr(IP);
  header_ip->daddr = inet_addr(IP);

  if (connect(sockfd, (struct sockaddr *)&server_addr, server_addr_len) == -1)
    handle_error("connect");

  memmove(buf + sizeof(struct udphdr) + sizeof(struct iphdr), msg, strlen(msg));
  if (sendto(sockfd, buf, BUF_SIZE, 0, (struct sockaddr *)&server_addr,
             server_addr_len) == -1)
    handle_error("sendto");

  memset(buf, '\0', BUF_SIZE);
  while (1) {
    if (recvfrom(sockfd, buf, BUF_SIZE, 0, (struct sockaddr *)&server_addr,
                 &server_addr_len) == -1)
      handle_error("recvfrom");
    header_udp = (struct udphdr *)(buf + 20);
    if (header_udp->dest == htons(PORT)) break;
  }

  printf("message from server : %s\n",
         buf + sizeof(struct udphdr) + sizeof(struct iphdr));

  close(sockfd);

  exit(EXIT_SUCCESS);
}